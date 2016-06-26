/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la lógica del juego.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/

#include <cstdlib>
#include <cassert>
#include <cstdio>

#include "Server.h"

#include "Logic/Events/EventManager.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/TimeManager.h"
#include "Logic/Maps/Managers/WorkManager.h"
#include "Logic/Maps/Managers/PowerManager.h"
#include "Logic/ResourcesManager.h"
#include "AI/Manager/AIManager.h"
#include "Logic/BuildingManager.h"
#include "Logic/SoulManager.h"
#include "Logic/Maps/Managers/GameManager.h"
#include "Logic/SoulsTrialManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "AI/Server.h"
#include "Map/MapParser.h"
#include "Map/MapEntity.h"

// HACK. Debería leerse de algún fichero de configuración
#define MANAGERS_FILE_PATH "./media/maps/"

#define MAX_LINE 500

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
		// Aleatorizamos la semilla de los números pseudoaleatorios
		std::srand((int)std::time(0));

		// Determinamos el entorno de ejecución del juego
		initGameRuntimeContext();

		// Inicializamos el parser de mapas.
		if (!Map::CMapParser::Init())
			return false;

		// Inicializamos la factoría de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;

		// Inicializamos el manager del tiempo.
		if (!Logic::CTimeManager::Init())
			return false;

		// Inicializamos el gestor de la matriz de tiles.
		if (!Logic::CTileManager::Init())
			return false;

		// Inicializamos el gestor del trabajo.
		if (!Logic::CWorkManager::Init())
			return false;

		// Inicializamos el gestor de edificios.
		if (!Logic::CBuildingManager::Init())
			return false;

		// Inicializamos el gestor de almas.
		if (!Logic::CSoulManager::Init())
			return false;

		// Inicializamos el gestor de almas.
		if (!Logic::CSoulsTrialManager::Init())
			return false;

		// Inicializamos el gestor de eventos de juego.
		if (!Logic::CEventManager::Init())
			return false;

		// Inicializamos el gestor de IA.
		if (!AI::CAIManager::Init())
			return false;

		// Inicializamos el manager de la energía.
		if (!Logic::CPowerManager::Init())
			return false;

		// Inicializamos el manager de partida.
		if (!Logic::CGameManager::Init())
			return false;

		// Inicializamos el manager de recursos.
		if (!Logic::ResourcesManager::Init())
			return false;

		return true;

	} // open

	void CServer::initGameRuntimeContext() {
		// Por defecto contexto de juego
		_gameRuntimeContext = GameRuntimeContext::GAME;

		std::ifstream in("GameRuntimeContext.cfg");
		if (in.good()) {
			char line[MAX_LINE];

			do {
				in.getline(line, MAX_LINE);
			} while (line[0] == '/');

			if (strcmp(line, "DEV") == 0)
				_gameRuntimeContext = GameRuntimeContext::DEV;
			else if (strcmp(line, "SCRIPTED_DEMO") == 0)
				_gameRuntimeContext = GameRuntimeContext::SCRIPTED_DEMO;
		}

		switch (_gameRuntimeContext) {
		case DEV:
			std::cout << "Running in DEV mode" << std::endl;
			break;

		case SCRIPTED_DEMO:
			std::cout << "Running in SCRIPTED_DEMO mode" << std::endl;
			break;

		case GAME:
			std::cout << "Running in GAME mode" << std::endl;
			break;

		default:
			assert(false && "Unkonw game runtime");
		}
	}

	//--------------------------------------------------------

	void CServer::close() 
	{
		unLoadLevel();

		Logic::CGameManager::Release();

		Logic::CPowerManager::Release();

		AI::CAIManager::Release();

		Logic::CEventManager::Release();

		Logic::CSoulsTrialManager::Release();

		Logic::CSoulManager::Release();

		Logic::CBuildingManager::Release();

		Logic::CWorkManager::Release();

		Logic::CTileManager::Release();

		Logic::CTimeManager::Release();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();

	} // close

	//--------------------------------------------------------

	bool CServer::loadManagersConfigurationValues(const std::string &filename)
	{
		// Completamos la ruta con el nombre de fichero dado
		std::string completePath(MANAGERS_FILE_PATH);
		completePath = completePath + filename;

		// Parseamos el fichero
		if (!Map::CMapParser::getSingletonPtr()->parseFile(completePath))
		{
			assert(!"No se ha podido parsear el fichero de configuración de managers.");
			return false;
		}

		// Extraemos las entidades del parseo...
		Map::CMapParser::TEntityList managersList =
			Map::CMapParser::getSingletonPtr()->getEntityList();

		// ...y cargamos los valores leídos para cada manager
		Map::CMapParser::TEntityList::const_iterator it, end;
		it = managersList.begin();
		end = managersList.end();

		for (; it != end; ++it) {
			Map::CEntity *manager = *it;

			if (manager->getType() == "SoulsTrialManager")
				CSoulsTrialManager::getSingletonPtr()->spawn(manager);

			if (manager->getType() == "AIManager")
				AI::CAIManager::getSingletonPtr()->spawn(manager);

			if (manager->getType() == "TimeManager")
				CTimeManager::getSingletonPtr()->spawn(manager);
		}

		return true;

	} // loadManagersConfigurationValues

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

		Logic::CTimeManager::getSingletonPtr()->tick(msecs);

		Logic::CSoulsTrialManager::getSingletonPtr()->tick(msecs);

		Logic::CEventManager::getSingletonPtr()->tick(msecs);

		AI::CAIManager::getSingletonPtr()->tick(msecs);

		Logic::CPowerManager::getSingletonPtr()->tick(msecs);
	
		Logic::CGameManager::getSingletonPtr()->tick(msecs);

	} // tick

	GameRuntimeContext CServer::getGameRuntimeContext() const {
		return _gameRuntimeContext;
	}

} // namespace Logic
