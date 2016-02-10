/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "Server.h"

#include "Logic\Entity\Components\Tile.h"
#include "Logic\Maps\Managers\TileManager.h"

#include <assert.h>

namespace AI {
	/////////////////////////////////////////
	/**
	Instancia única de la clase.
	*/
	CServer* CServer::_instance = 0;
	/////////////////////////////////////////
	/**
	Constructor de la clase.
	*/
	CServer::CServer(void)
	{
		assert(!_instance && "Segunda inicialización de AI::CServer no permitida!");
		_fWalkingSoul = new CAStarFunctionsWalkingSoul();
		_fSoulPath = new CAStarFunctionsSoulPath();
		_aStarWalkingSoul = new micropather::MicroPather(_fWalkingSoul);
		_aStarSoulPath = new micropather::MicroPather(_fSoulPath);
		_tileManager = Logic::CTileManager::getSingletonPtr();
	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CServer::~CServer(void)
	{
		assert(_instance);
		delete _aStarSoulPath;
		delete _aStarWalkingSoul;
		delete _fSoulPath;
		delete _fWalkingSoul;
	}
	/////////////////////////////////////////
	/**
	Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
	una vez durante la inicialización de la aplicación. 

	@return Devuelve false si no se ha podido inicializar.
	*/
	bool CServer::Init() 
	{
		_instance = new CServer();
		return true;
	}
	/////////////////////////////////////////
	/**
	Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
	*/
	void CServer::Release()
	{
		if (_instance)
			delete _instance;
		_instance = 0;
	}

	/////////////////////////////////////////

	/**
	Calcula una ruta de soulpaths usando A* desde un cierto tile a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getSoulPathAStarRoute(Logic::Tile* from, Logic::Tile* to){
		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarSoulPath->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>(path->size());
		out->push_back(from);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back((Logic::Tile*)(*it));
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Calcula una ruta para almas caminantes usando A* desde un cierto tile a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getWalkingSoulAStarRoute(Logic::Tile* from, Logic::Tile* to){
		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarWalkingSoul->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>(path->size());
		out->push_back(from);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back((Logic::Tile*)(*it));
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
	*/
	double CServer::correctAngle(double angle)
	{
		while (angle > Math::PI)
			angle -= 2 * Math::PI;
		while (angle < -Math::PI)
			angle += 2 * Math::PI;
		return angle;
	}

} // namespace AI