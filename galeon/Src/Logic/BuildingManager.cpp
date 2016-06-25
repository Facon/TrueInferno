//---------------------------------------------------------------------------
// BuildingManager.cpp
//---------------------------------------------------------------------------

/**
@file BuildingManager.cpp

Contiene la implementación del gestor de la matriz de tiles.

@see Logic::CBuildingManager

@author Álvaro Valera
@date Enero, 2016
*/

#include "BuildingManager.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Message.h"
#include "Logic\Maps\Managers\TileManager.h"
#include "Logic\Entity\Components\Tile.h"

#include <cassert>

namespace Logic {

	CBuildingManager* CBuildingManager::_instance = 0;

	//--------------------------------------------------------

	CBuildingManager::CBuildingManager()
	{
		_instance = this;
		_buildings = std::map<BuildingType, std::set<CPlaceable*>*>();

		_buildingGroups = std::map<BuildingGroup, std::set<BuildingType>*>();
		_buildingGroups[PowerSupply] = new std::set<BuildingType>{ PowerGenerator };
		_buildingGroups[MapResources] = new std::set<BuildingType> { Mine, GasPlant };
		_buildingGroups[Production] = new std::set<BuildingType> { Furnace };
		_buildingGroups[EvilProcessing] = new std::set<BuildingType>{ EvilWorks, Refinery };
		_buildingGroups[Logistics] = new std::set<BuildingType>{ Warehouse, Evilator };
		_buildingGroups[IDi] = new std::set<BuildingType> { ResearchLabs };

	} // CBuildingManager

	//--------------------------------------------------------

	CBuildingManager::~CBuildingManager()
	{
		assert(_instance);

		// Liberamos la estructura de datos para los grupos de edificios por tipo
		for (auto it = _buildingGroups.begin(); it != _buildingGroups.end(); ++it)
		{
			it->second->clear();

			delete it->second;
			it->second = nullptr;
		}

		// Liberamos la  estructura de datos para los edificios
		for (auto it = _buildings.begin(); it != _buildings.end(); ++it)
		{
			for (auto it2 = (*it).second->cbegin(); it2 != (*it).second->cend(); ++it2)
			{
				delete (*it2);
			}

			it->second->clear();

			delete it->second;
			it->second = nullptr;
		}

		_buildings.clear();

		_instance = nullptr;
	} // ~CBuildingManager

	//--------------------------------------------------------

	bool CBuildingManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CBuildingManager no permitida!");

		new CBuildingManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CBuildingManager::Release()
	{
		assert(_instance && "Logic::CBuildingManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CBuildingManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CBuildingManager::close()
	{
	} // close

	bool CBuildingManager::loadInitialBuildings(CMap *map){
		// Coge la Map::CEntity "Tile" leída del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una función genérica que reciba el nombre de la Map::CEntity.
		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		// TODO Colocamos temporalmente hardcodeando posiciones en código
		//CEntity* evilator = createPlaceable(map, "Evilator", Vector3(5, 0, 12));
		CEntity* evilator = createPlaceable(map, "Evilator", Vector3(18, 0, 20), false, false);
		if (!evilator)
			return false;

		CEntity* hellQuarters = createPlaceable(map, "HellQuarters", Vector3(24, 0, 23), false, false);
		if (!hellQuarters)
			return false;

		// Carreteras iniciales
		/*
		if(!createPlaceable(map, "SoulPath", Vector3(13, 0, 4)))
			return false;

		if (!createPlaceable(map, "SoulPath", Vector3(13, 0, 5)))
			return false;

		for (int x = 13; x >= 5; --x)
			if (!createPlaceable(map, "SoulPath", Vector3(x, 0, 6)))
				return false;

		for (int z = 7; z <= 11; ++z)
			if (!createPlaceable(map, "SoulPath", Vector3(5, 0, z)))
				return false;
		*/

		// Obstáculos iniciales
		CEntity* ruinas = createPlaceable(map, "Ruinas", Vector3(3, 0, 8), false, false);
		if (!ruinas)
			return false;

		ruinas = createPlaceable(map, "Ruinas", Vector3(13, 0, 29), false, false);
		if (!ruinas)
			return false;

		ruinas = createPlaceable(map, "Ruinas", Vector3(35, 0, 40), false, false);
		if (!ruinas)
			return false;

		return true;
	}

	void CBuildingManager::registerBuilding(CPlaceable *building){
		// Ignoramos todo lo que no sean edificios
		if (!building->isBuilding())
			return;

		// Obtenemos el tipo del edificio
		BuildingType buildingType = building->getBuildingType();

		// Almacenaremos en el mapa indexando por el tipo de edificio

		// Creamos, si no existe, el índice para el tipo de edificio
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		if (buildingsFromType == nullptr){
			buildingsFromType = new std::set<CPlaceable*>();
			_buildings[buildingType] = buildingsFromType;
		}

		// Añadimos el edificio
		buildingsFromType->insert(building);

		//std::cout << "Building registered: " << building->getBuildingType() << std::endl;
	}

	void CBuildingManager::unregisterBuilding(CPlaceable *building){
		// Ignoramos todo lo que no sean edificios
		if (!building->isBuilding())
			return;

		// Obtenemos el tipo del edificio
		BuildingType buildingType = building->getBuildingType();

		// Eliminamos el puntero al edificio en el conjunto de edificios para ese tipo
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		if (buildingsFromType != nullptr){
			buildingsFromType->erase(building);

			// Si era el último de su tipo eliminamos el set
			if (buildingsFromType->empty()){
				_buildings.erase(buildingType);
				delete buildingsFromType;
				buildingsFromType = nullptr;
			}
		}

		//std::cout << "Building unregistered: " << building->getBuildingType() << std::endl;
		//printBuildingList();
	}

	CEntity* CBuildingManager::createPlaceable(CMap *map, const std::string& prefabName, const Vector3& logicPosition, bool floating, bool showFloating){
		bool ret = true;
		
		// Primero se intenta crear la entidad
		CEntity* newEntity = CEntityFactory::getSingletonPtr()->createEntity(prefabName, map);
		if (newEntity){
			// En segundo lugar se desplaza
			ret &= floatPlaceableTo(newEntity, logicPosition, showFloating);

			// Por último, salvo que sea flotante, lo intentamos colocar en su posición
			if (!floating)
				ret &= placePlaceable(newEntity);
		}
		else{
			ret = false;
		}

		// Si algo falló
		if (!ret){
			//std::cout << "Can't create new placeable '"<< prefabName <<"' on '"<< logicPosition <<"'" << std::endl;

			// Eliminamos la instancia si se llegó a crear
			if (newEntity)
				CEntityFactory::getSingletonPtr()->deleteEntity(newEntity);

			return nullptr;
		}

		return newEntity;
	}

	void CBuildingManager::destroyPlaceable(CEntity *entity, bool withEffects){
		// Si usamos efectos, pedimos al placeable que se destruya
		if (withEffects) {
			CPlaceable* placeable = entity->getComponent<CPlaceable>();
			if (placeable != nullptr){
				placeable->destroyWithEffects();
				return;
			}

			else
				assert(false && "DestroyPlaceable used on non-Placeable entity");
		}

		// Se elimina la entidad inmediatamente. OJO: deleteDeferred no eliminaba correctamente la entidad
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(entity);

		// Nota: No hace falta desregistrar porque se hace automáticamente en el destructor de Placeable
	}

	bool CBuildingManager::floatPlaceableTo(CEntity* movableEntity, const Vector3& logicPosition, bool showFloating){
		if (!movableEntity){
			std::cout << "Can't float null placeable to '" << logicPosition << "'" << std::endl;
			return false;
		}

		MovePlaceableMessage m(logicPosition, showFloating);

		return m.Dispatch(*movableEntity);
	}

	bool CBuildingManager::placePlaceable(CEntity* movableEntity)
	{
		if (!movableEntity)
		{
			std::cout << "Can't place null placeable" << std::endl;
			return false;
		}
		
		MovePlaceableMessage m;

		return m.Dispatch(*movableEntity);
	}

	bool CBuildingManager::checkValidPlaceablePosition(CEntity* placeableEntity, const Vector3& logicPosition){
		if (!placeableEntity){
			std::cout << "Can't check null placeable to'" << logicPosition << "'" << std::endl;
			return false;
		}

		CheckValidPositionPlaceableMessage m(MessageType::PLACEABLE_CHECKPOSITION, placeableEntity, logicPosition);
		 
		return m.Dispatch(*placeableEntity);
	}

	int CBuildingManager::getBuildingTypeNumber(BuildingType buildingType)
	{
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		return (buildingsFromType == nullptr) ? 0 : buildingsFromType->size();
	}

	std::set<CPlaceable*>* CBuildingManager::getBuildingsFromType(BuildingType buildingType)
	{
		return _buildings[buildingType];
	}

	CPlaceable* CBuildingManager::findBuilding(BuildingType buildingType){
		// Obtenemos el conjunto de edificios para el tipo
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		if (buildingsFromType == nullptr || buildingsFromType->size() == 0)
			return nullptr;

		// Seleccionamos uno al azar
		int numBuildings = buildingsFromType->size();
		int randomIndex = rand() % numBuildings;
		auto it = buildingsFromType->cbegin();
		std::advance(it, randomIndex);
		return *it;
	}

	CPlaceable* CBuildingManager::getRandomBuilding(){
		// Obtenemos un tipo aleatorio
		int randomIndex = rand() % _buildings.size();
		auto it = _buildings.cbegin();
		std::advance(it, randomIndex);
		return findBuilding(it->first);
	}

	CPlaceable* CBuildingManager::getRandomBuildingforDestruction(){
		// Obtenemos un tipo aleatorio
		if (checkValidBuildingTypeforDestruction())
		{
			int randomIndex;
			auto it = _buildings.cbegin();
			do
			{
				it = _buildings.cbegin();
				randomIndex = rand() % _buildings.size();
				
				std::advance(it, randomIndex);
			}
			while ((it->first == BuildingType::Evilator || randomIndex == BuildingType::HellQuarters || randomIndex == BuildingType::NonBuilding) || it->second == nullptr || it->second->size() == 0);

			return findBuilding(it->first);
		}
		return nullptr;
	}

	void CBuildingManager::printBuildingList() const{
		std::cout << "Buildings=" << std::endl;
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			std::cout << CPlaceable::printBuildingType(it->first) << ": " << it->second->size() << std::endl;

			/*for (auto it2 = it->second->cbegin(); it2 != it->second->cend(); ++it2){
				std::cout << (*it2)->getEntity()->getEntityID() << std::endl;
			}*/
		}
	}

	bool CBuildingManager::checkValidBuildingTypeforDestruction() 
	{
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			if (it->first != BuildingType::Evilator && it->first != BuildingType::HellQuarters && it->first != BuildingType::NonBuilding)
			{
				if (it->second != nullptr && it->second->size() > 0)
					return true;
			}
		}
		return false;
	}

	bool CBuildingManager::DestroyRandomBuilding(){
		CPlaceable* building = getRandomBuildingforDestruction();
		if (building != nullptr){
			// Invocamos la destrucción del placeable. Internamente se harán todas las operaciones necesarias para asegurar la consistencia de los datos
			destroyPlaceable(building->getEntity(), true);
			return true;
		}
		return false;
	}

	std::map<BuildingGroup, std::set<BuildingType>*>& CBuildingManager::getBuildingGroups()
	{
		return _buildingGroups;
	}

	std::map<BuildingType, std::set<CPlaceable*>*>& CBuildingManager::getBuildings()
	{
		return _buildings;
	}

	bool CBuildingManager::HandleMessage(const LogisticsMessage& msg){
		bool ret = false;

		// Para cada tipo de edificio registrado
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			// Para cada uno de sus edificios
			for (auto it2 = it->second->cbegin(); it2 != it->second->cend(); ++it2){
				// Se reenvía el mensaje a la entidad del edificio
				ret |= (*it2)->getEntity()->HandleMessage(msg);
			}
		}

		return ret;
	}

	bool CBuildingManager::HandleMessage(const ResourceMessage& msg){
		bool ret = false;

		// Para cada tipo de edificio registrado
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			// Para cada uno de sus edificios
			if (it->second != nullptr)
				for (auto it2 = it->second->cbegin(); it2 != it->second->cend(); ++it2){
					// Se reenvía el mensaje a la entidad del edificio
					ret |= (*it2)->getEntity()->HandleMessage(msg);
				}
		}

		return ret;
	}

	//--------------------------------------------------------

	bool CBuildingManager::growDestroyableObstacle(CMap* map, const Vector3& position){
		CTileManager* tileManager = CTileManager::getSingletonPtr();

		Tile* tile = tileManager->getTile(position);

		// Intentamos colocar obstáculo
		//return (createPlaceable(map, "Tree", position, false, false) != nullptr);
		return (createPlaceable(map, "Ruinas", position, false, false) != nullptr);
	}

	//--------------------------------------------------------

/*
	void CBuildingManager::growRandomDestroyableObstacles(int number){
		// Obtenemos el número de tiles
		int numTiles = CTileManager:: SIZE_X * SIZE_Z;

		// Determinamos cuántas transformaremos en obstáculos
		int numObstacles = numTiles * factor;

		// Intentamos transformar tiles aleatorias
		for (int i = 0; i < numObstacles; ++i){
			Tile* tile = getRandomTile();

			// Si la tile no tiene placeable y su terreno está vacío
			if (tile->getPlaceableAbove() == nullptr){
				CBuildingManager::
			}
		}
*/

	//--------------------------------------------------------

	void CBuildingManager::growDestroyableObstaclesRandomly(CMap* map, int number){
		CTileManager* tileManager = CTileManager::getSingletonPtr();

		// Mientras queden obstáculos que poner
		while (number > 0){
			// Obtenemos la posición de una tile aleatoria
			Tile* tile = tileManager->getRandomTile();
			Vector3 pos = tile->getLogicPosition();

			// Intentamos colocar obstáculo
			if (!growDestroyableObstacle(map, pos)){
				// Si no se pudo, decrementamos el contador para evitar tardar demasiado
				--number;
				continue;
			}
			
			// Intentamos colocar más obstáculos en posiciones contiguas
			/*
			do {
				pos.x += (-1 + rand() % 3);
				pos.z += (-1 + rand() % 3);
				--number;
			} while (growDestroyableObstacle(map, pos) && number > 0);
			*/
			--number;
		}
	}
	
} // namespace Logic
