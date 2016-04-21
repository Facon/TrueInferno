#include "Placeable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Events/EventManager.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/BuildingManager.h"
#include "Logic/Entity/PlaceableType.h"
#include "AI/Server.h"
#include "Logic/Entity/Message.h"

#include <iostream>
#include <string>

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPlaceable);
	IMP_FACTORY(CPlaceable);

	CPlaceable::CPlaceable() : IComponent() {
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

		return true;
	} // spawn

	void CPlaceable::tick(unsigned int msecs){
		//processWalkingSoulPathRequest();

	} // tick

	bool CPlaceable::place() {
		// Si no estábamos flotando no hacemos nada porque ya estábamos (teóricamente bien) colocados
		if (!_floating)
			return true;

		// Check if current tiles allow the placement
		bool check = checkPlacementIsPossible(_floorOriginPosition);
		if (!check)
			return false;

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

		// Cambiamos la altura a justo encima de la tile
		Vector3& position = _entity->getPosition();

		// @TODO Es necesario incluir aquí una distinción para todos los edificios que tengan un
		// modelo propio con el pivote "cercano" al centro, pero no justo en el centro...
		if (_placeableType == Building)
		{
			if (getBuildingType() == Furnace) position.y -= _entity->getDimensions().y / 3;
			else if (getBuildingType() == Refinery) position.y -= _entity->getDimensions().y / 2;
			else if (_placeableType == Building) position.y -= _entity->getDimensions().y / 2;
		}
		else if (_placeableType == SoulPath)
		{
			position.y -= 0.5f;
		}

		PositionMessage m(position);
		m.Dispatch(*_entity);

		// Cambiamos el estado
		_floating = false;

		// Eventos del tutorial
		// @TODO Hacer bien...
		if (isBuilding()) {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
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

		// Clear tile vectors
		_tiles.clear();
		_adyacentTiles.clear();

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

		// Get the average position
		centerPosition /= _tiles.size();

		// Añadimos cierta altura a la posición del Placeable para que parezca que está colocada encima o sobrevolando la Tile
		centerPosition += Vector3(0, HEIGHT_ON_TILE + _entity->getDimensions().y / 2, 0);

		// Move entity physically
		_entity->setPosition(centerPosition);

		// Update adyacent tiles
		updateAdyacentTiles();
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

	bool CPlaceable::ConsumeResourcesForConstruction(){

		Logic::ResourcesManager* resourcesManager = ResourcesManager::getSingletonPtr();

		// Chequeeamos inicialmente los costes para todos los recursos
		int finalCost;
		if (!resourcesManager->decrementResources(ResourceType::MINERAL, _mineralCost, true, false, finalCost))
			return false;
		if (!resourcesManager->decrementResources(ResourceType::GAS, _gasCost, true, false, finalCost))
			return false;

		// Computamos los costes
		assert(resourcesManager->decrementResources(ResourceType::MINERAL, _mineralCost, false, false, finalCost) && "Can't pay mineral costs");
		assert(resourcesManager->decrementResources(ResourceType::GAS, _gasCost, false, false, finalCost) && "Can't pay gas costs");

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

} // namespace Logic