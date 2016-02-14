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

		return true;
	}

	void CBuildingManager::registerBuilding(CPlaceable *placeable, const BuildingType& buildingType){
		// Ignoramos todo lo que no sean edificios
		if (!placeable->isBuilding())
			return;

		std::cout << "Registering " << buildingType << std::endl;

		// TODO Store in a map by type
		_buildings.push_back(placeable);
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

		// Si algo falló
		if (!ret){
			std::cout << "Can't create new placeable '"<< prefabName <<"' on '"<< logicPosition <<"'" << std::endl;

			// Eliminamos la instancia si se llegó a crear
			if (newEntity)
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(newEntity);

			return nullptr;
		}

		return newEntity;
	}

} // namespace Logic
