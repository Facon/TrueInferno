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

#include <cassert>

namespace Logic {

	CBuildingManager* CBuildingManager::_instance = 0;

	//--------------------------------------------------------

	CBuildingManager::CBuildingManager()
	{
		_instance = this;
		_buildings = std::vector<CPlaceable*>();

	} // CBuildingManager

	//--------------------------------------------------------

	CBuildingManager::~CBuildingManager()
	{
		assert(_instance);
		_instance = 0;
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

	void CBuildingManager::loadInitialBuildings(CMap *map){
		// Coge la Map::CEntity "Tile" leída del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una función genérica que reciba el nombre de la Map::CEntity.
		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();


		// Localización de los prefabs necesarios
		// TODO Generalizar lectura y almacenaje de prefabs de edificio (prefab=true && tiene componente Placeable)
		Map::CEntity *prefabHellQuarters = nullptr;
		Map::CEntity *prefabEvilator = nullptr;

		for (auto it = mapEntityList.begin(); it != mapEntityList.end(); ++it) {
			if ((*it)->getType() == "HellQuarters") {
				prefabHellQuarters = *it;
			}
			else if ((*it)->getType() == "Evilator") {
				prefabEvilator = *it;
			}
		}

		// Construimos HellQuarters
		// TODO Generalizar construcción de edificios en una función que reciba el nombre del prefab y la posición lógica
		prefabHellQuarters->setAttribute("prefab", "false");
		prefabHellQuarters->setAttribute("floor_absolute_position0", "0 0 0");
		prefabHellQuarters->setName(prefabHellQuarters->getName() + "0");
		if (entityFactory->createEntity(prefabHellQuarters, map)){
			std::cout << "HellQuarters created!" << std::endl;
		}
		else{
			std::cout << "Can't create HellQuarters :(" << std::endl;
		}

		// Construimos Evilator
		// TODO Generalizar construcción de edificios en una función que reciba el nombre del prefab y la posición lógica
		prefabEvilator->setAttribute("prefab", "false");
		prefabEvilator->setAttribute("floor_absolute_position0", "6 0 6");
		prefabEvilator->setName(prefabEvilator->getName() + "0");
		if (entityFactory->createEntity(prefabEvilator, map)){
			std::cout << "Evilator created!" << std::endl;
		}
		else{
			std::cout << "Can't create Evilator :(" << std::endl;
		}
	}

	void CBuildingManager::registerBuilding(CPlaceable *placeable){
		_buildings.push_back(placeable);
	}

} // namespace Logic
