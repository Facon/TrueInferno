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
#include "AI/Server.h"
#include "Logic/Entity/Message.h"

#include <cassert>

namespace Logic {

	CBuildingManager* CBuildingManager::_instance = 0;

	//--------------------------------------------------------

	CBuildingManager::CBuildingManager()
	{
		_instance = this;
		_buildings = std::map<BuildingType, std::set<CPlaceable*>*>();

	} // CBuildingManager

	//--------------------------------------------------------

	CBuildingManager::~CBuildingManager()
	{
		assert(_instance);
		_instance = 0;

		// Liberamos la  estructura de datos para los edificios
		for (auto it = _buildings.begin(); it != _buildings.end(); ++it){
			delete it->second;
			it->second = nullptr;
		}
		_buildings.clear();

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
		CEntity* evilator = createPlaceable(map, "Evilator", Vector3(8, 0, 4), false, false);
		if (!evilator)
			return false;

		CEntity* hellQuarters = createPlaceable(map, "HellQuarters", Vector3(12, 0, 4), false, false);
		if (!hellQuarters)
			return false;

		// Carretera inicial
		/*if(!createPlaceable(map, "SoulPath", Vector3(13, 0, 4)))
			return false;

		if (!createPlaceable(map, "SoulPath", Vector3(13, 0, 5)))
			return false;

		for (int x = 13; x >= 5; --x)
			if (!createPlaceable(map, "SoulPath", Vector3(x, 0, 6)))
				return false;

		for (int z = 7; z <= 11; ++z)
			if (!createPlaceable(map, "SoulPath", Vector3(5, 0, z)))
				return false;*/

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
			if (buildingsFromType->empty())
				_buildings.erase(buildingType);
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
			std::cout << "Can't create new placeable '"<< prefabName <<"' on '"<< logicPosition <<"'" << std::endl;

			// Eliminamos la instancia si se llegó a crear
			if (newEntity)
				CEntityFactory::getSingletonPtr()->deleteEntity(newEntity);

			return nullptr;
		}

		return newEntity;
	}

	void CBuildingManager::destroyPlaceable(CEntity *entity){
		// Se elimina la entidad inmediatamente. OJO: NO usar deleteDeferred porque, por algún motivo, no elimina correctamente la entidad
		CEntityFactory::getSingletonPtr()->deleteEntity(entity);

		// No hace falta desregistrar porque se hace automáticamente en el destructor de Placeable
	}

	bool CBuildingManager::floatPlaceableTo(CEntity* movableEntity, const Vector3& logicPosition, bool showFloating){
		if (!movableEntity){
			std::cout << "Can't float null placeable to '" << logicPosition << "'" << std::endl;
			return false;
		}

		MovePlaceableMessage m(logicPosition, showFloating);

		return m.Dispatch(*movableEntity);
	}

	bool CBuildingManager::placePlaceable(CEntity* movableEntity){
		if (!movableEntity){
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


	int CBuildingManager::getBuildingTypeNumber(BuildingType buildingType){
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		return (buildingsFromType == nullptr) ? 0 : buildingsFromType->size();
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
			destroyPlaceable(building->getEntity());
			return true;
		}
		return false;
	}

	std::map<BuildingType, std::set<CPlaceable*>*>& CBuildingManager::getBuildings(){
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

} // namespace Logic
