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

	bool CBuildingManager::loadInitialBuildings(CMap *map){
		// Coge la Map::CEntity "Tile" leída del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una función genérica que reciba el nombre de la Map::CEntity.
		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		// TODO Colocamos temporalmente hardcodeando posiciones en código
		CEntity* evilator = createBuilding(map, "Evilator", Vector3(5, 0, 14));
		if (!evilator)
			return false;

		CEntity* hellQuarters = createBuilding(map, "HellQuarters", Vector3(13, 0, 2));
		if (!hellQuarters)
			return false;

		return true;
	}

	void CBuildingManager::registerBuilding(CPlaceable *placeable){
		_buildings.push_back(placeable);
	}

	CEntity* CBuildingManager::createBuilding(CMap *map, const std::string& prefabName, const Vector3& logicPosition){
		CEntity* buildingEntity = CEntityFactory::getSingletonPtr()->createEntity(prefabName, map);

		// TODO Apaño temporal: Buscamos a capón la entidad recién creada (con posición por defecto) en el array de buildings
		// Habría que enviar un mensaje a la entidad para que se ubique en la posición lógica dada (CPlaceable::place())
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			CPlaceable* building = (*it);
			if (building->getEntity() == buildingEntity){
				(*it)->place(logicPosition);
				break;
			}
		}

		if (!buildingEntity){
			std::cout << "Can't create new building '"<< prefabName <<"' on '"<< logicPosition <<"'" << std::endl;
			return nullptr;
		}

		return buildingEntity;
	}

} // namespace Logic
