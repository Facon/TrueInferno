//---------------------------------------------------------------------------
// BuildingManager.cpp
//---------------------------------------------------------------------------

/**
@file BuildingManager.cpp

Contiene la implementaci�n del gestor de la matriz de tiles.

@see Logic::CBuildingManager

@author �lvaro Valera
@date Enero, 2016
*/

#include "BuildingManager.h"
#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "AI/Server.h"

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
		assert(!_instance && "Segunda inicializaci�n de Logic::CBuildingManager no permitida!");

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
		assert(_instance && "Logic::CBuildingManager no est� inicializado!");

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
		// Coge la Map::CEntity "Tile" le�da del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una funci�n gen�rica que reciba el nombre de la Map::CEntity.
		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		// TODO Colocamos temporalmente hardcodeando posiciones en c�digo
		CEntity* evilator = createPlaceable(map, "Evilator", Vector3(5, 0, 12));
		if (!evilator)
			return false;

		CEntity* hellQuarters = createPlaceable(map, "HellQuarters", Vector3(13, 0, 2));
		if (!hellQuarters)
			return false;

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

		AI::CServer::getSingletonPtr()->getWalkingSoulAStarRoute(hellQuarters, evilator);

		return true;
	}

	void CBuildingManager::registerBuilding(CPlaceable *building){
		// Ignoramos todo lo que no sean edificios
		if (!building->isBuilding())
			return;

		// Obtenemos el tipo del edificio
		BuildingType buildingType = building->getBuildingType();

		// Almacenaremos en el mapa indexando por el tipo de edificio

		// Creamos, si no existe, el �ndice para el tipo de edificio
		std::set<CPlaceable*>* buildingsFromType = _buildings[buildingType];
		if (buildingsFromType == nullptr){
			buildingsFromType = new std::set<CPlaceable*>();
			_buildings[buildingType] = buildingsFromType;
		}

		// A�adimos el edificio
		buildingsFromType->insert(building);
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
		}
	}

	CEntity* CBuildingManager::createPlaceable(CMap *map, const std::string& prefabName, const Vector3& logicPosition){
		bool ret = false;
		
		// Primero se intenta crear la entidad
		CEntity* newEntity = CEntityFactory::getSingletonPtr()->createEntity(prefabName, map);
		if (newEntity){
			// En segundo lugar se posiciona mediante paso de mensaje
			PlaceMessage m;
			m.position = logicPosition;
			ret = m.Dispatch(*newEntity);
		}

		// Si algo fall�
		if (!ret){
			std::cout << "Can't create new placeable '"<< prefabName <<"' on '"<< logicPosition <<"'" << std::endl;

			// Eliminamos la instancia si se lleg� a crear
			if (newEntity)
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(newEntity);

			return nullptr;
		}

		return newEntity;
	}

	void CBuildingManager::printBuildingList() const{
		for (auto it = _buildings.begin(); it != _buildings.end(); ++it){
			std::cout << "BuildingType=" << it->first << std::endl;

			for (auto it2 = it->second->cbegin(); it2 != it->second->cend(); ++it2){
				std::cout << (*it2)->getEntity()->getEntityID() << std::endl;
			}
		}
	}

} // namespace Logic
