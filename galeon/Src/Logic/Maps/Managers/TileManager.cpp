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

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"

#include <cassert>
#include <fstream>

namespace Logic {

	CTileManager* CTileManager::_instance = 0;

	//--------------------------------------------------------

	CTileManager::CTileManager()
	{
		_instance = this;
		_tiles = nullptr;

	} // CTileManager

	//--------------------------------------------------------

	CTileManager::~CTileManager()
	{
		assert(_instance);
		_instance = 0;

		// Free tile matrix
		if (_tiles){
			for (int x = 0; x < SIZE_X; ++x){
				if (_tiles[x]){
					delete(_tiles[x]);
					_tiles[x] = nullptr;
				}
			}
			delete(_tiles);
			_tiles = nullptr;
		}

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
		Map::CEntity *mapEntityTile;

		Map::CMapParser::TEntityList mapEntityList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		Map::CMapParser::TEntityList::const_iterator it, end;
		it = mapEntityList.begin();
		end = mapEntityList.end();

		// Find Tile "prefab"
		for (; it != end; it++) {
			if ((*it)->getType() == "Tile") {
				mapEntityTile = *it;
			}
		}

		assert(mapEntityTile && "Map::CEntity Tile not found");

		// Genera todas las Logic::CEntity tiles de la matriz a partir de la
		// Map::CEntity "Tile" leída.
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		Vector3 tileBasePosition = mapEntityTile->getVector3Attribute("position");
		std::string baseName = mapEntityTile->getStringAttribute("name");

		// Allocate memory for tile matrix
		_tiles = new Tile**[SIZE_X];
		for (int x = 0; x < SIZE_X; ++x) {
			_tiles[x] = new Tile*[SIZE_Z];
			for (int z = 0; z < SIZE_Z; ++z) {
				_tiles[x][z] = nullptr;
			}
		}

		// Read the tile map
		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				// Change attribute position.
				Vector3 tilePosition(tileBasePosition);
				tilePosition.x = (float) x;
				tilePosition.y = (float) 0; // Currently we only manage a 2D tile matrix
				tilePosition.z = (float) z;

				// Build new Vector3::position attribute with the logic position in the matrix: "x y z"
				// @TODO This should be done inside Map::MapEntity
				std::stringstream newPosition;
				newPosition << tilePosition.x << " " << tilePosition.y << " " << tilePosition.z;
				mapEntityTile->setAttribute("position", newPosition.str());

				// Change attribute name (must be unique).
				std::stringstream newTileName;
				newTileName << baseName;
				newTileName << "_" << tilePosition.x << "_" << tilePosition.z;
				mapEntityTile->setName(newTileName.str());

				// Change prefab attribute
				mapEntityTile->setAttribute("prefab", "false");

				// Create a new entity Tile.
				CEntity *entityTile = entityFactory->createEntity(mapEntityTile, map);
				assert(entityTile && "Failed to create entity Tile[X,Z]");
			}
		}

		// Read the terrain configuration
		loadTerrain(TERRAIN_MAP_FILE);

	} // loadInitialMatrix

	//--------------------------------------------------------

	Tile* CTileManager::getTile(const Vector3 &position){
		return _tiles[(int)position.x][(int)position.z];
	}

	//--------------------------------------------------------

	void CTileManager::registerTile(Tile *tile){
		Vector3 position = tile->getLogicPosition();
		_tiles[(int)position.x][(int)position.z] = tile;
	}

	//--------------------------------------------------------

	void CTileManager::loadTerrain(const std::string &filename){
		// Open file
		std::ifstream file(filename);

		// For each line
		std::string line;
		int x = 0;
		while (std::getline(file, line)) {
			// Read line
			std::stringstream ss(line);
			
			// For each token in line delimited by whitespace
			int z = 0;
			std::string item;
			while (std::getline(ss, item, ' ')) {
				assert((item.size() == 1) && "Only tokens of 1 char are allowed in the terrain map");
				
				TerrainType terrainType;
				switch (item[0]){
				case '0':
					terrainType = TerrainType::Empty;
					break;
				case '1':
					terrainType = TerrainType::Mineral;
					break;
				case '2':
					terrainType = TerrainType::Gas;
					break;
				default:
					assert("Unknown terrain type char: "+terrainType);
				}

				_tiles[x][z]->setTerrainType(terrainType);
				++z;
			}
			++x;
		}

		file.close();
	}

	bool CTileManager::open(){
		return true;
	} // open

	//--------------------------------------------------------

	void CTileManager::close(){
	} // close

} // namespace Logic
