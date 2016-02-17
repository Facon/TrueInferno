/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#include "Server.h"

#include "Logic\Entity\Components\Tile.h"
#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Maps\Managers\TileManager.h"

#include <assert.h>

namespace AI {
	/////////////////////////////////////////
	/**
	Instancia �nica de la clase.
	*/
	CServer* CServer::_instance = 0;
	/////////////////////////////////////////
	/**
	Constructor de la clase.
	*/
	CServer::CServer(void)
	{
		assert(!_instance && "Segunda inicializaci�n de AI::CServer no permitida!");
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
	Inicializa el servidor de IA. Esta operaci�n s�lo es necesario realizarla
	una vez durante la inicializaci�n de la aplicaci�n. 

	@return Devuelve false si no se ha podido inicializar.
	*/
	bool CServer::Init() 
	{
		_instance = new CServer();
		return true;
	}
	/////////////////////////////////////////
	/**
	Libera el servidor de IA. Debe llamarse al finalizar la aplicaci�n.
	*/
	void CServer::Release()
	{
		if (_instance)
			delete _instance;
		_instance = 0;
	}

	/////////////////////////////////////////

	/**
	Calcula con A* una ruta de soulpaths desde un cierto Tile a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getSoulPathAStarRoute(Logic::Tile* from, Logic::Tile* to){
		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar �nicamente cuando se modifique el mapa para las soulpaths: Soulpath eliminada (las constru�das no cambian nada), edificio constru�do sobre una casilla vac�a, etc.
		_aStarSoulPath->Reset();

		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarSoulPath->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>();
		out->push_back(from);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back((Logic::Tile*)(*it));
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Calcula con A* una ruta para que las almas vayan caminando desde un cierto Tile a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getWalkingSoulAStarRoute(Logic::Tile* from, Logic::Tile* to){
		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar �nicamente cuando se modifique el mapa para las almas: Soulpath eliminada o constru�da, etc.
		_aStarWalkingSoul->Reset();

		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarWalkingSoul->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>();
		out->push_back(from);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back((Logic::Tile*)(*it));
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Calcula con A* una ruta para que las almas vayan caminando desde un cierto Placeable a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getWalkingSoulAStarRoute(Logic::CPlaceable* from, Logic::CPlaceable* to){
		// Obtenemos el conjunto de tiles objetivo
		std::unordered_set<Logic::Tile*> tiles = to->getAdyacentTiles();
		std::unordered_set<void*> tos = std::unordered_set<void*>();
		for (auto it = tiles.cbegin(); it != tiles.cend(); ++it){
			tos.insert((void*)(*it));
		}

		// Obtenemos la tile de origen
		// TODO Pasar el conjunto completo de tiles adyacentes al placeable
		Logic::Tile* tileFrom = *(from->getAdyacentTiles().cbegin());

		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar �nicamente cuando se modifique el mapa para las almas: Soulpath eliminada o constru�da, etc.
		_aStarWalkingSoul->Reset();

		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarWalkingSoul->Solve((void*)tileFrom, tos, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>();
		out->push_back(tileFrom);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			out->push_back((Logic::Tile*)(*it));
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Dado un �ngulo en radianes lo lleva al intervalo [-PI, PI]
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