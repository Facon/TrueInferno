#include "Placeable.h"

#include <iostream>
#include <string>

#include "Map/MapEntity.h"

#include "Logic/Server.h"
#include "Logic/ResourcesManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/BuildingManager.h"
#include "Logic/Entity/PlaceableType.h"
#include "Logic/Entity/Message.h"
#include "Logic/HFManager.h"
#include "Logic/Entity/ParticleType.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPlaceable);
	IMP_FACTORY(CPlaceable);

	CPlaceable::CPlaceable() : IComponent(), _hadesFavorReward(0), _defaultMaterial(""), 
		_doingGraphicPlace(false), _doingGraphicFloat(false), 
		_placeSpeed(PLACE_SPEED), _floatSpeed(FLOAT_SPEED),
		_smokeConstructionDuration(2000), _destroying(false),
		_destructionDuration(5000), _destructionSpeed(PLACE_SPEED) {
	}

	CPlaceable::~CPlaceable() {
		// Si es un edificio
		if (_placeableType == Building)
			// Lo desregistramos en el manager
			Logic::CBuildingManager::getSingletonPtr()->unregisterBuilding(this);

		// Y eliminamos este placeable de sus tiles para que no comprueben conflicto de posición
		for (auto it = _tiles.cbegin(); it != _tiles.cend(); ++it){
			// Sólo eliminamos si somos nosotros
			if ((*it)->getPlaceableAbove() == this)
				(*it)->setPlaceableAbove(nullptr);
		}

		_tiles.clear();
		_adyacentTiles.clear();
		clearFloatingVisualEntities();
	}

	bool CPlaceable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
	
		// Inicialización
		_tileManager = nullptr;
		_tiles.clear();
		_adyacentTiles.clear();
		_placeableType = NonPlaceable;
		_buildingType = Unassigned;
		_walkingSoulTarget = nullptr;
		_validTerrainTypes.clear();

		// TODO Remove when this check is done above
		if (entityInfo->hasAttribute("prefab") && entityInfo->getBoolAttribute("prefab"))
			return true;

		// Inicialmente los edificios flotan
		_floating = true;

		// Get TileManager's reference
		_tileManager = Logic::CTileManager::getSingletonPtr();

		// Initialiate absolute origin position
		_floorOriginPosition = 0;

		// Read floor's size
		if (!entityInfo->hasAttribute("dimensions")){
			assert(false && "Dimensions must be defined");
			return false;
		}

		//extraemos los costes

		if (entityInfo->hasAttribute("mineralCost"))
			_mineralCost = entityInfo->getIntAttribute("mineralCost");

		if (entityInfo->hasAttribute("gasCost"))
			_gasCost = entityInfo->getIntAttribute("gasCost");

		if (entityInfo->hasAttribute("hadesFavorReward"))
			_hadesFavorReward = entityInfo->getIntAttribute("hadesFavorReward");

		// Chequeamos que las dimensiones X,Z sean números enteros
		Vector3 dimensions = entityInfo->getVector3Attribute("dimensions");
		if (dimensions.x != round(dimensions.x) || dimensions.z != round(dimensions.z)){
			assert(false && "Dimensions X,Z must be integer");
			return false;
		}
		_floorX = dimensions.x;
		_floorZ = dimensions.z;

		// Determinamos el tipo de placeable que es
		_placeableType = parsePlaceableType(entityInfo->getStringAttribute("placeableType"));

		// Si el edificio fue creado por prefab, es posible que al hacer spawn no tenga el atributo de posición lógica inicial
		if (entityInfo->hasAttribute("floor_absolute_position0")){
			// Hacemos flotar al edificio hasta su posición inicial
			Vector3 initialPosition = entityInfo->getVector3Attribute("floor_absolute_position0");
			floatTo(initialPosition, true);

			// Get memory for all the tiles
			_tiles = std::vector<Tile*>(_floorX * _floorZ);
		}

		// Si es un edificio
		if (_placeableType == Building){
			// Determinamos el tipo
			_buildingType = parseBuildingType(entityInfo->getStringAttribute("type"));
		}
		else
			_buildingType = NonBuilding;

		// Leemos los tipos de terreno válidos
		if (!entityInfo->hasAttribute("validTerrainType")){
			std::cout << "validTerrainType must be defined" << std::endl;
			return false;
		}
		_validTerrainTypes.clear();
		_validTerrainTypes.insert(parseTerrainType(entityInfo->getStringAttribute("validTerrainType")));

		if (!entityInfo->hasAttribute("material")){
			assert(false && "Placeable must have a material");
			return false;
		}
		_defaultMaterial = entityInfo->getStringAttribute("material");

		return true;
	} // spawn

	void CPlaceable::tick(unsigned int msecs){
		// Si se está destruyendo no se hace otra cosa
		if (_destroying){
			doGraphicDestroy(msecs);
			return;
		}
		
		// En otro caso vemos si está flotando o si está siendo colocado
		if (_doingGraphicFloat){
			doGraphicFloat(msecs);
		}

		else if (_doingGraphicPlace){
			doGraphicPlace(msecs);
		}

	} // tick

	bool CPlaceable::place() {
		// Si no estábamos flotando no hacemos nada porque ya estábamos (teóricamente bien) colocados
		if (!_floating)
			return true;

		// Check if current tiles allow the placement
		bool check = checkPlacementIsPossible(_floorOriginPosition);
		if (!check)
			return false;

		// Eliminamos la entidad de feedback
		clearFloatingVisualEntities();

		// For each of our tiles
		for (auto it = _tiles.cbegin(); it != _tiles.cend(); ++it){
			assert("Can't place if there's a Placeable!" && (*it)->getPlaceableAbove()==nullptr);

			// Notify of the new entity above
			(*it)->setPlaceableAbove(this);
		}

		// Registramos el edificio en el buildingManager
		if (_placeableType == Building){
			Logic::CBuildingManager::getSingletonPtr()->registerBuilding(this);
		}

		// Partimos de la posición de justo encima del tile. Usamos la posición lógica donde tiene que acabar flotando la entidad
		Vector3& position = _targetFloatPosition;

		// @TODO Es necesario incluir aquí una distinción para todos los edificios que tengan un
		// modelo propio con el pivote "cercano" al centro, pero no justo en el centro...
		if (_placeableType == Building)
		{
			if (getBuildingType() == HellQuarters) position.y = HEIGHT_ON_TILE - 0.3f;
			else if (getBuildingType() == Evilator) position.y = HEIGHT_ON_TILE - 0.2f;
			else if (getBuildingType() == Mine) position.y = HEIGHT_ON_TILE - 0.2f;
			else if (getBuildingType() == Furnace) position.y = HEIGHT_ON_TILE - 0.3f;
			else if (getBuildingType() == Refinery) position.y = HEIGHT_ON_TILE + 0.15f;
			else if (getBuildingType() == EvilWorks) position.y = HEIGHT_ON_TILE - 0.8f;
			else if (getBuildingType() == Warehouse) position.y = HEIGHT_ON_TILE - 0.5f;
			else if (getBuildingType() == PowerGenerator) position.y = HEIGHT_ON_TILE - 0.1f;
			else if (_placeableType == Building) position.y = HEIGHT_ON_TILE;
		}
		else if (_placeableType == SoulPath)
		{
			position.y = 0.6f;
		}
		else if (_placeableType == Obstacle)
		{
			position.y = HEIGHT_ON_TILE - 0.4;
		}

		// Guardamos la posición a la que queremos que se haga el place
		_targetPlacePosition = position;
		_doingGraphicPlace = true;

		// Cambiamos el estado
		_floating = false;

		// Eventos del tutorial
		// @TODO Hacer bien...
		if (isBuilding()) {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
		}

		// Incrementamos el favor de Hades por construcción de edificio
		HFManager* hadesFavorManager = HFManager::getSingletonPtr();
		hadesFavorManager->changeHadesFavor(_hadesFavorReward);

		// Activamos sonido de construcción
		SoundMessage soundMessage(MessageType::SOUND_PLAY, "building_complete");
		soundMessage.Dispatch(*_entity);

		// Ponemos el material básico
		MaterialMessage materialMessage(_defaultMaterial);
		bool result = materialMessage.Dispatch(*_entity);
		assert(result && "Can't set default material");

		// Forzamos rechequeo del estado de habilitado de la entidad para pintar de oscuro el material
		ToggleMessage toggleMessage = ToggleMessage();
		toggleMessage.Dispatch(*_entity);

		// Activamos partículas de construcción en los edificios
		if (isBuilding()) {
			ParticleMessage particleMessage(ParticleType::CONSTRUCTION_SMOKE, 0);
			result = particleMessage.Dispatch(*_entity);
			assert(result && "Can't start building construction particles");
		}

		return true;
	}

	void CPlaceable::floatTo(const Vector3 newOriginPosition, bool showFloating){
		//std::cout << "Flotando a: " << newOriginPosition << std::endl;

		if (newOriginPosition.x != round(newOriginPosition.x) || newOriginPosition.z != round(newOriginPosition.z)){
			assert(false && "Ignoring floatTo to a non-integer [X,Z] position");
			return;
		}
		
		// Si no estábamos flotando
		if (!_floating){
			// Hacemos que flote
			_floating = true;

			// Desregistramos el edificio del buildingManager para que no esté accesible
			if (_placeableType == Building){
				Logic::CBuildingManager::getSingletonPtr()->unregisterBuilding(this);
			}

			// Y eliminamos este placeable de sus tiles para que no comprueben conflicto de posición
			for (auto it = _tiles.cbegin(); it != _tiles.cend(); ++it){
				// Sólo eliminamos si somos nosotros
				if((*it)->getPlaceableAbove() == this)
					(*it)->setPlaceableAbove(nullptr);
			}
		}

		// Don't do anything if the origin position is not changing (and tiles have been already initialitated)
		if ((newOriginPosition == _floorOriginPosition) && _tiles.capacity()>0)
			return;

		// Store new origin position
		_floorOriginPosition = newOriginPosition;

		// Clear tiles and adyacent tiles vectors
		_tiles.clear();
		_adyacentTiles.clear();

		// Clear visual feedback entities created for floating buildings
		clearFloatingVisualEntities();

		// Initialize vector to calculate the average position of all tiles
		Vector3 centerPosition(0, 0, 0);

		// Update tiles based on the Placeable's floor size
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));

				// Añadimos la Tile si no es null (i.e. se salió de los bordes)
				if (tile != nullptr){
					// Store it internally
					_tiles.push_back(tile);

					// Add it to the average position
					centerPosition += tile->getEntity()->getPosition();
				}
			}
		}

		// Calculate the average if there were valid tiles
		if (!_tiles.empty())
			centerPosition /= _tiles.size();
		
		// Or place it in the origin position
		else
			centerPosition = _floorOriginPosition;

		// Añadimos cierta altura a la posición del Placeable para que parezca que está colocada encima o sobrevolando la Tile
		float buildingHeightIncrement = 0.0f;

		// Es necesario incluir aquí una distinción para todos los edificios que tengan un
		// modelo propio con el pivote "cercano" al centro, pero no justo en el centro...
		if (_placeableType == Building)
		{
			if (getBuildingType() == Furnace) buildingHeightIncrement = HEIGHT_ON_TILE - 0.3;
			else if (getBuildingType() == Refinery) buildingHeightIncrement = HEIGHT_ON_TILE;
			else if (getBuildingType() == EvilWorks) buildingHeightIncrement = HEIGHT_ON_TILE - 0.8f;
			else if (getBuildingType() == Warehouse) buildingHeightIncrement = HEIGHT_ON_TILE - 0.5f;
			else if (getBuildingType() == PowerGenerator) buildingHeightIncrement = HEIGHT_ON_TILE - 0.1f;
			else if (_placeableType == Building) buildingHeightIncrement = HEIGHT_ON_TILE;
		}
		else if (_placeableType == SoulPath)
		{
			buildingHeightIncrement = 0.3f;
		}
		else if (_placeableType == Obstacle)
		{
			buildingHeightIncrement = HEIGHT_ON_TILE - 0.4;
		}

		float centerHeight = (showFloating) ? HEIGHT_ON_TILE + buildingHeightIncrement : 0.6f;
		centerPosition += Vector3(0, centerHeight, 0);

		// Guardamos la posición a la que queremos que se haga el float
		_targetFloatPosition = centerPosition;
		_doingGraphicFloat = true;

		// Update adyacent tiles
		updateAdyacentTiles();

		if (_buildingType == BuildingType::Evilator){
			bool test = true;
		}

		// Ponemos un material u otro según estemos en una posición válida o no
		std::string buildingPlacementMaterial;

		if (checkPlacementIsPossible(_floorOriginPosition))
			buildingPlacementMaterial = "Building/RightPlaced";
		else
			buildingPlacementMaterial = "Building/WrongPlaced";

		MaterialMessage m(buildingPlacementMaterial);
		const bool result = m.Dispatch(*_entity);
		assert(result && "Can't set right/wrong placed material");

		// Feedback gráfico para las tiles sobre las que flota el Placeable
		std::string tilePlacementMaterial;

		if (buildingPlacementMaterial == "Building/RightPlaced")
			tilePlacementMaterial = "Tile/RightPlaced";
		else
			tilePlacementMaterial = "Tile/WrongPlaced";

		if (showFloating && _buildingType != BuildingType::NonBuilding)
		{
			for (int x = 0; x < _floorX; ++x) {
				for (int z = 0; z < _floorZ; ++z) {
					Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));
					if (tile != nullptr){
						CEntity *visualPlaceable = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(),
							"SoulPath", tile->getLogicPosition(), true, false);

						visualPlaceable->getComponent<Logic::CGraphics>()->setMaterialName(tilePlacementMaterial);
						_floatingVisualEntities.push_back(visualPlaceable);
					}
				}
			}
		}
	}
	
	bool CPlaceable::checkPlacementIsPossible(const Vector3 &checkPosition) const{
		// For each tile
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(checkPosition + Vector3(x, 0, z));

				// Check tile
				if (tile == nullptr || !tile->allowsPlaceable(this))
					return false;
			}
		}

		// All checks are done!
		return true;
	}

	void CPlaceable::clearFloatingVisualEntities()
	{
		while (!_floatingVisualEntities.empty())
		{
			CEntity *entity = _floatingVisualEntities.back();
			_floatingVisualEntities.pop_back();

			Logic::CEntityFactory::getSingletonPtr()->deleteEntity(entity);
			entity = nullptr;
		}

		_floatingVisualEntities.clear();
	}

	const std::vector<Tile*> CPlaceable::getTiles() const{
		return _tiles;
	}

	const std::unordered_set<Tile*> CPlaceable::getAdyacentTiles() const{
		return _adyacentTiles;
	}

	bool CPlaceable::canPassSoulPath() const{
		// Sólo permitimos pasar SoulPath si hay SoulPath
		return _placeableType == SoulPath;
	}

	bool CPlaceable::canPassWalkingSoul() const{
		// Sólo permitimos a las almas caminar si hay SoulPath. Podría interesar dejarlas atravesar también los edificios
		return _placeableType == SoulPath;
	}

	bool CPlaceable::canBePlacedOnTerrain(TerrainType terrainType) const{
		return _validTerrainTypes.count(terrainType)>0;
	}

	bool CPlaceable::isBuilding() const{
		return _placeableType == Building;
	}

	bool CPlaceable::isSoulPath() const{
		return _placeableType == SoulPath;
	}

	bool CPlaceable::HandleMessage(const MovePlaceableMessage& msg){
		if (msg._type == MessageType::PLACEABLE_FLOAT_TO){
			floatTo(msg._position, msg._showFloating);
			return true;
		}

		else if (msg._type == MessageType::PLACEABLE_PLACE){
			return place();
		}

		else{
			assert(false && "Unimplemented routine for this MessageType");
			return false;
		}
	}

	void CPlaceable::destroyWithEffects(){
		// Activamos las partículas de destrucción
		ParticleMessage pm(ParticleType::DESTRUCTION_SMOKE, 0);
		bool result = pm.Dispatch(*_entity);
		assert(result && "Can't start destruction particles");

		// Desactivamos el edificio para que ya no pueda interactuar con él nadie
		ToggleMessage tm(LogicRequirement::Player, true);
		result = tm.Dispatch(*_entity);
		assert(result && "Can't toggle off building");

		// Activamos el flag para destruirse
		_destroying = true;

		// Establecemos una posición de destino para la destrucción que esté muy abajo
		_targetDestroyPosition = _entity->getPosition() + Vector3(0, -1000, 0); // OMG: NÚMERO MÁGICO DE ALBERTO!
	}

	bool CPlaceable::ConsumeResourcesForConstruction(){

		Logic::ResourcesManager* resourcesManager = ResourcesManager::getSingletonPtr();

		// Chequeeamos inicialmente los costes para todos los recursos
		int finalCost;
		if (!resourcesManager->decrementResources(ResourceType::MINERAL, _mineralCost, true, false, finalCost))
			return false;
		if (!resourcesManager->decrementResources(ResourceType::GAS, _gasCost, true, false, finalCost))
			return false;

		// Computamos los costes
		bool result = resourcesManager->decrementResources(ResourceType::MINERAL, _mineralCost, false, false, finalCost);
		assert(result && "Can't pay mineral costs");
		
		result = resourcesManager->decrementResources(ResourceType::GAS, _gasCost, false, false, finalCost);
		assert(result && "Can't pay gas costs");

		return true;
	}


	bool CPlaceable::HandleMessage(const GetCostPlaceableMessage& msg){
		if (msg._type == MessageType::PLACEABLE_CONSUME_COST){
			return ConsumeResourcesForConstruction();
		}
		else{
			assert(false && "Unimplemented routine for this MessageType");
			return false;
		}
	}

	PlaceableType CPlaceable::parsePlaceableType(const std::string& name){
		if (name == "Obstacle"){
			return Obstacle;
		}
		else if (name == "Building"){
			return Building;
		}
		else if (name == "SoulPath"){
			return SoulPath;
		}
		else{
			assert(false && "PlaceableType name unknown");
			// TODO lanzar excepción en vez de assert y eliminar valor NoPlaceable
			return NonPlaceable;
		}
	}

	BuildingType CPlaceable::parseBuildingType(const std::string& name){
		if (name == "HellQuarters"){
			return HellQuarters;
		}
		else if (name == "Furnace"){
			return Furnace;
		}
		else if (name == "EvilWorks"){
			return EvilWorks;
		}
		else if (name == "Refinery"){
			return Refinery;
		}
		else if (name == "Evilator"){
			return Evilator;
		}
		else if (name == "Mine"){
			return Mine;
		}
		else if (name == "GasPlant"){
			return GasPlant;
		}
		else if (name == "ResearchLabs"){
			return ResearchLabs;
		}
		else if (name == "PowerGenerator"){
			return PowerGenerator;
		}
		else if (name == "Warehouse"){
			return Warehouse;
		}
		else{
			assert(false && "BuildingType name unknown");
			// TODO lanzar excepción en vez de assert
			return Unassigned;
		}
	}

	std::string CPlaceable::printBuildingType(BuildingType type){
		if (type == BuildingType::HellQuarters){
			return "HellQuarters";
		}
		else if (type == BuildingType::Furnace){
			return "Furnace";
		}
		else if (type == BuildingType::EvilWorks){
			return "EvilWorks";
		}
		else if (type == BuildingType::Refinery){
			return "Refinery";
		}
		else if (type == BuildingType::Evilator){
			return "Evilator";
		}
		else if (type == BuildingType::Mine){
			return "Mine";
		}
		else if (type == BuildingType::GasPlant){
			return "GasPlant";
		}
		else if (type == BuildingType::ResearchLabs){
			return "ResearchLabs";
		}
		else if (type == BuildingType::PowerGenerator){
			return "PowerGenerator";
		}
		else if (type == BuildingType::Warehouse){
			return "Warehouse";
		}
		else{
			return "unknown";
		}
	}

	TerrainType CPlaceable::parseTerrainType(const std::string& name){
		if (name == "Empty"){
			return Empty;
		}
		else if (name == "Mineral"){
			return Mineral;
		}
		else if (name == "Gas"){
			return Gas;
		}
		else{
			assert(false && "TerrainType name unknown");
			// TODO lanzar excepción en vez de assert
			return Empty;
		}
	}

	bool CPlaceable::HandleMessage(const CheckValidPositionPlaceableMessage& msg){
		if (msg._type == MessageType::PLACEABLE_CHECKPOSITION){
			return checkPlacementIsPossible(_floorOriginPosition);
		}
		return false;
	}


	void CPlaceable::updateAdyacentTiles(){
		// For each tile
		for (auto itTiles = _tiles.cbegin(); itTiles != _tiles.cend(); ++itTiles){
			Tile *tile = (*itTiles);

			// For each of its adyacent tiles
			for (auto itAdyacent = tile->getAdjacentTiles()->cbegin(); itAdyacent != tile->getAdjacentTiles()->cend(); ++itAdyacent){
				Tile *adyacentTile = (*itAdyacent);

				// Save adyacent tile (* which may belong to the Placeable's tile. Read below)
				_adyacentTiles.insert(*itAdyacent);
			}
		}

		// As a tile is adyacent to the placeable's tiles we have to remove them
		// (* Here we check the set once for each of the placeable's tiles. This way we avoid checking the tile vector for each tile-adyacent)
		for (auto itTiles = _tiles.cbegin(); itTiles != _tiles.cend(); ++itTiles){
			Tile *tile = (*itTiles);
			_adyacentTiles.erase(tile);
		}
	}

	void CPlaceable::doGraphicFloat(int msecs) {
		// Flotamos si es necesario
		if (_doingGraphicFloat)
			doImmediateGraphicMovement(_targetFloatPosition, _doingGraphicFloat);
	}

	void CPlaceable::doGraphicPlace(int msecs) {
		// Colocamos si es necesario
		if (_doingGraphicPlace)
			doDelayedGraphicMovement(msecs, _targetPlacePosition, _placeSpeed, _doingGraphicPlace);
		else
			return;

		// Si acabó ya la colocación
		if (!_doingGraphicPlace) {
			// Dejamos las partículas de humo de construcción un tiempo para los edificios
			if (isBuilding()) {
				ParticleMessage particleMessage(ParticleType::CONSTRUCTION_SMOKE, _smokeConstructionDuration);
				bool result = particleMessage.Dispatch(*_entity);
				assert(result && "Can't stop building construction particles");
			}
		}
	}

	void CPlaceable::doGraphicDestroy(int msecs) {
		// Enterramos el edificio si no llegamos a la posición
		if (!_destroying){
			assert(false && "Calling doGraphicDestroy on a building that's not being destroyed");
			return;
		}
			
		// Reducimos el tiempo que queda para la destrucción
		_destructionDuration -= msecs;

		// Si se acabó: destrucción!
		if (_destructionDuration <= 0)
			// Delegamos en el manager pero ya SIN efectos
			CBuildingManager::getSingletonPtr()->destroyPlaceable(_entity, false);

		// Si no, seguimos moviendo al edificio
		else
			doDelayedGraphicMovement(msecs, _targetDestroyPosition, _placeSpeed, _destroying);
	}

	void CPlaceable::doDelayedGraphicMovement(int msecs, const Vector3& targetPosition, float speed, bool& moving) {
		if (!moving)
			return;

		Vector3 currentPosition = _entity->getPosition();
		Vector3 dir = targetPosition - currentPosition;

		if (dir.squaredLength() <= ZERO_DISTANCE){
			moving = false;
			return;
		}

		dir.normalise();
		dir *= speed * msecs;

		currentPosition += dir;

		PositionMessage m(currentPosition);
		bool result = m.Dispatch(*_entity);
		assert(result && "Can't do animated graphic move");
	}

	void CPlaceable::doImmediateGraphicMovement(const Vector3& targetPosition, bool& moving) {
		if (!moving)
			return;

		PositionMessage positionMessage(targetPosition);
		bool result = positionMessage.Dispatch(*_entity);

		assert(result && "Can't move immediate graphic move");

		if (result)
			moving = false;
	}

} // namespace Logic