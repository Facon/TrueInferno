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
		CEntity* evilator = createBuilding(map, "Evilator", Vector3(5, 0, 12));
		if (!evilator)
			return false;

		CEntity* hellQuarters = createBuilding(map, "HellQuarters", Vector3(13, 0, 2));
		if (!hellQuarters)
			return false;

		if(!createBuilding(map, "SoulPath", Vector3(13, 0, 4)))
			return false;

		if (!createBuilding(map, "SoulPath", Vector3(13, 0, 5)))
			return false;

		for (int x = 13; x >= 5; --x)
			if (!createBuilding(map, "SoulPath", Vector3(x, 0, 6)))
				return false;

		for (int z = 7; z <= 11; ++z)
			if (!createBuilding(map, "SoulPath", Vector3(5, 0, z)))
				return false;

		return true;
	}

	void CBuildingManager::registerBuilding(CPlaceable *placeable){
		_buildings.push_back(placeable);
	}

	CEntity* CBuildingManager::createSoulPath(CMap *map, const Vector3& logicPosition){
		CEntity* soulPath = CEntityFactory::getSingletonPtr()->createEntity("SoulPath", map);
		if (soulPath){
			PlaceMessage m;
			m.position = logicPosition;
			m.Dispatch(*soulPath);
		}
		return soulPath;
	}

	CEntity* CBuildingManager::createBuilding(CMap *map, const std::string& prefabName, const Vector3& logicPosition){
		CEntity* buildingEntity = CEntityFactory::getSingletonPtr()->createEntity(prefabName, map);

		// TODO Apa�o temporal para ubicar el edificio: Buscamos a cap�n la entidad reci�n creada (con posici�n por defecto) en el array de buildings
		// Habr�a que enviar un mensaje a la entidad para que se ubique en la posici�n l�gica dada (CPlaceable::place())
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			CPlaceable* building = (*it);

			// Si es la entidad buscada
			if (building->getEntity() == buildingEntity){
				// Intentamos ubicarla
				if (!(*it)->place(logicPosition)){
					// Destru�mos la entidad si no se pudo ubicar
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(buildingEntity);
					buildingEntity = nullptr;
				}
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
