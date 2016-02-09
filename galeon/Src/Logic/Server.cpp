/**
@file Server.cpp

Contiene la implementaci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la l�gica del juego.

@see Logic::CServer

@author David Llans�
@date Agosto, 2010
*/

#include "Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/Managers/BuildingManager.h"

#include "Logic/Entity/Entity.h"

#include "AI/Server.h"

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
		assert(!_instance && "Segunda inicializaci�n de Logic::CServer no permitida!");

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
		assert(_instance && "Logic::CServer no est� inicializado!");

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

		// Inicializamos la factor�a de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;

		// Inicializamos el gestor de la matriz de tiles.
		if (!Logic::CTileManager::Init())
			return false;

		// Inicializamos el gestor de edificios.
		if (!Logic::CBuildingManager::Init())
			return false;

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		unLoadLevel();

		Logic::CBuildingManager::Release();

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

		// Cargamos la matriz de tiles inicial en el mapa.
		if (!CTileManager::getSingletonPtr()->loadInitialMatrix(_map)){
			std::cout << "Error loading initial buildings" << std::endl;
			return false;
		}

		// Cargamos la lista de edificios inicial en el mapa.
		if (!CBuildingManager::getSingletonPtr()->loadInitialBuildings(_map)){
			std::cout << "Error loading initial buildings" << std::endl;
			return false;
		}

		//CTileManager::getSingletonPtr()->printDebugInfo();

		// Cargamos el mapa de navegaci�n
		AI::CServer::getSingletonPtr()->computeNavigationGraph();

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

} // namespace Logic
