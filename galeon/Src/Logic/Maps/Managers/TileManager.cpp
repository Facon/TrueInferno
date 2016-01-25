//---------------------------------------------------------------------------
// TileManager.cpp
//---------------------------------------------------------------------------

/**
@file TileManager.cpp

Contiene la implementación del gestor de la matriz de tiles.

@see Logic::CTileManager

@author Raúl Segura
@date Enero, 2016
*/

#include "TileManager.h"

#include "Logic/Maps/EntityFactory.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include <cassert>

namespace Logic {

	CTileManager* CTileManager::_instance = 0;

	//--------------------------------------------------------

	CTileManager::CTileManager()
	{
		_instance = this;

	} // CTileManager

	//--------------------------------------------------------

	CTileManager::~CTileManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CTileManager

	//--------------------------------------------------------

	bool CTileManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CTileManager no permitida!");

		new CTileManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CTileManager::Release()
	{
		assert(_instance && "Logic::CTileManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CTileManager::loadInitialMatrix(CMap *map)
	{
		// Coge la Map::CEntity "Tile" leída del fichero de mapa a modo de prefab.
		// @TODO Hacerlo en Map::CParser mediante una función genérica que reciba el nombre de la Map::CEntity.
		Map::CEntity *mapEntityTile;

		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		Map::CMapParser::TEntityList::const_iterator it, end;
		it = mapEntityList.begin();
		end = mapEntityList.end();

		for (; it != end; it++) {
			if ((*it)->getType() == "Tile") {
				mapEntityTile = *it;
			}
		}

		assert(mapEntityTile && "Map::CEntity Tile not found");

		// Genera todas las Logic::CEntity tiles de la matriz a partir de la
		// Map::CEntity "Tile" leída del fichero de mapa.
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		Vector3 tileBasePosition = mapEntityTile->getVector3Attribute("position");
		Vector3 tileBaseDimensions = mapEntityTile->getVector3Attribute("dimensions");

		// Material base para los tiles.
		std::string baseMaterialName = mapEntityTile->getStringAttribute("material");
		// Material alternativo para los tiles.
		// Se usa para diferenciar visualmente tiles adyacentes entre sí.
		std::string altMaterialName = baseMaterialName + "_alt";

		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				// Cambia la posición.
				Vector3 tilePosition(tileBasePosition);
				tilePosition.x += x * tileBaseDimensions.x;
				tilePosition.z += z * tileBaseDimensions.z;

				// Construye la nueva posición como un Vector3 representado por un
				// string con formato: "x y z".
				// @TODO Esto debería hacerse en Map::MapEntity.
				std::stringstream newPosition;
				newPosition << tilePosition.x << " " << tilePosition.y << " " << tilePosition.z;

				mapEntityTile->setAttribute("position", newPosition.str());

				// Cambia el color de los tiles impares.
				mapEntityTile->setAttribute("material",
					(((x + z) % 2 == 0) ? baseMaterialName : altMaterialName));

				// Cambia el nombre (debe ser único!).
				std::stringstream newTileName;
				newTileName << mapEntityTile->getStringAttribute("name");
				newTileName << "_" << tilePosition.x << "_" << tilePosition.z;

				mapEntityTile->setName(newTileName.str());

				// Crea una nueva Logic::CEntity tile.
				CEntity *entityTile = entityFactory->createEntity(mapEntityTile, map);
				assert(entityTile && "Failed to create entity Tile[X,Z]");
			}
		}

	} // loadInitialMatrix

	//--------------------------------------------------------

	bool CTileManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CTileManager::close()
	{
	} // close

} // namespace Logic
