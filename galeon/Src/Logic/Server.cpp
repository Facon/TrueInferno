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

#include "Logic/Entity/Entity.h"

#include "Map/MapParser.h"

#include <cassert>

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

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		unLoadLevel();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();

	} // close

	//--------------------------------------------------------

	bool CServer::loadLevel(const std::string &filename)
	{
		// solo admitimos un mapa cargado, si iniciamos un nuevo nivel 
		// se borra el mapa anterior.
		unLoadLevel();

		_map = CMap::createMapFromFile(filename);

		if (!_map) {
			return false;
		}

		// #TODO Aqu� el mapa ya ha sido cargado de fichero y las entidades
		// del map.txt est�n le�das y creadas.

		// Se podr�a acceder a ellas a trav�s de alg�n atributo (iterando sobre la lista)
		// y definir los Tiles extremos, instanciando los intermedios
		// a partir de ellos, cambiando solo la posici�n y el ID.

		// La segunda opci�n es a�adirlas todas al vuelo, aqu� o en un manager.

		CEntity* baseTileEntity = _map->getEntityByType("Tile");

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
