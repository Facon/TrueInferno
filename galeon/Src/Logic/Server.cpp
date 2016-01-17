/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la lógica del juego.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Managers/TileManager.h"

#include "Logic/Entity/Entity.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include <cassert>
#include <cstdio>

namespace Logic {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _map(0)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		// Inicializamos el parser de mapas.
		if (!Map::CMapParser::Init())
			return false;

		// Inicializamos la factoría de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;

		// Inicializamos el gestor de la matriz de tiles.
		if (!Logic::CTileManager::Init())
			return false;

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		unLoadLevel();

		Logic::CTileManager::Release();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();

	} // close

	//--------------------------------------------------------

	bool CServer::loadLevel(const std::string &filename)
	{
		// Solo admitimos un mapa cargado, si iniciamos un nuevo nivel se
		// borra el mapa anterior.
		unLoadLevel();

		// Cargamos el fichero de mapa.
		_map = CMap::createMapFromFile(filename);

		if (!_map) {
			return false;
		}

		// Aquí el mapa ya ha sido cargado de fichero y las entidades del
		// map.txt están leídas y creadas.

		// Entre ellas, habrá una Map::CEntity Tile que puede ser empleada
		// de forma similar a un prefab para generar toda la matriz de
		// tiles inicial.

		// @TODO La generación de la matriz de tiles debería ser realizada
		// por el TileManager en función de la matriz de enteros a cargar.

		createTilesMatrix();
		return true;

	} // loadLevel

	//--------------------------------------------------------

	void CServer::unLoadLevel()
	{
		if(_map)
		{
			_map->deactivate();
			delete _map;
			_map = 0;
		}
		_player = 0;

	} // unLoadLevel

	//---------------------------------------------------------

	bool CServer::activateMap() 
	{
		return _map->activate();

	} // activateMap

	//---------------------------------------------------------

	void CServer::deactivateMap() 
	{
		_map->deactivate();

	} // deactivateMap

	//---------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		// Eliminamos las entidades que se han marcado para ser eliminadas.
		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();

		_map->tick(msecs);

	} // tick

	//--------------------------------------------------------

	void CServer::createTilesMatrix()
	{
		// Cogemos la Map::CEntity Tile leída del fichero de mapa a modo de prefab.
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

		// Generamos todas las Logic:CEntity (tiles) de la matriz a partir de la
		// Map::CEntity leída.
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		Vector3 tileBasePosition = mapEntityTile->getVector3Attribute("position");

		for (int x = 0; x < SIZE_X; ++x) {
			for (int z = 0; z < SIZE_Z; ++z) {
				// Change attribute position.
				Vector3 tilePosition(tileBasePosition);
				tilePosition.x += x;
				tilePosition.z += z;

				// Build new Vector3::position attribute: "x y z"
				// @TODO This should be done inside Map::MapEntity
				std::stringstream newPosition;
				newPosition << tilePosition.x << " " << tilePosition.y << " " << tilePosition.z;

				mapEntityTile->setAttribute("position", newPosition.str());

				// Change attribute name (must be unique).
				std::stringstream newTileName;
				newTileName << mapEntityTile->getStringAttribute("name");
				newTileName << "_" << tilePosition.x << "_" << tilePosition.z;

				mapEntityTile->setName(newTileName.str());

				// Create a new entity Tile.
				CEntity *entityTile = entityFactory->createEntity(mapEntityTile, _map);
				assert(entityTile && "Failed to create entity Tile[X,Z]");
			}
		}

	} // createTilesMatrix

} // namespace Logic
