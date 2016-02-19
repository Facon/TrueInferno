/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "Server.h"

#include "Logic\Entity\Components\Tile.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Maps\Managers\TileManager.h"

#include <unordered_set>
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
	Calcula con A* una ruta de soulpaths desde un cierto Tile a otro.
	*/
	std::vector<Logic::Tile*>* CServer::getSoulPathAStarRoute(Logic::Tile* from, Logic::Tile* to){
		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar únicamente cuando se modifique el mapa para las soulpaths: Soulpath eliminada (las construídas no cambian nada), edificio construído sobre una casilla vacía, etc.
		_aStarSoulPath->Reset();

		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarSoulPath->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		// Transformamos el retorno
		std::vector<Logic::Tile*>* out = new std::vector<Logic::Tile*>();
		out->push_back(from);
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			Logic::Tile* tile = (Logic::Tile*)(*it);
			out->push_back(tile);
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Calcula con A* una ruta para que las almas vayan caminando desde un cierto Tile a otro.
	*/
	std::vector<Vector3>* CServer::getWalkingSoulAStarRoute(Logic::Tile* from, Logic::Tile* to){
		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar únicamente cuando se modifique el mapa para las almas: Soulpath eliminada o construída, etc.
		_aStarWalkingSoul->Reset();

		std::vector<void*>* path = new std::vector<void*>();
		float cost = 0.0f;
		int solved = _aStarWalkingSoul->Solve((void*)from, (void*)to, path, &cost);
		if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
			delete path;
			return NULL;
		}

		// Transformamos el retorno
		std::vector<Vector3>* out = new std::vector<Vector3>();

		// ¿Es necesario añadir la posición actual del Tile?
		// out->push_back(from->getEntity()->getPosition());
		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++) {
			Logic::Tile* tile = (Logic::Tile*)(*it);
			out->push_back(tile->getEntity()->getPosition());
		}

		delete path;
		return out;
	}

	/////////////////////////////////////////

	/**
	Calcula con A* una ruta para que las almas vayan caminando desde un cierto Placeable a otro.
	*/
	std::vector<Vector3>* CServer::getWalkingSoulAStarRoute(Logic::CPlaceable* from, Logic::CPlaceable* to){
		// Obtenemos el conjunto de tiles objetivo
		std::unordered_set<Logic::Tile*> toTiles = to->getAdyacentTiles();
		std::unordered_set<void*> tos = std::unordered_set<void*>();
		for (auto it = toTiles.cbegin(); it != toTiles.cend(); ++it){
			// Introducimos cada Tile como estado
			tos.insert((void*)(*it));
		}

		// Obtenemos las tiles de origen
		std::unordered_set<Logic::Tile*> fromTiles = from->getAdyacentTiles();

		// Reseteamos para asegurarnos que los vecinos se recalculan por si el mapa ha cambiado
		// TODO Llamar únicamente cuando se modifique el mapa para las almas: Soulpath eliminada o construída, etc.
		_aStarWalkingSoul->Reset();

		// Probamos a obtener ruta desde cada Tile de origen para conseguir la mejor ruta
		// TODO Lo correcto sería meter el Placeable como estado inicial para expandirlo dentro. Para eso se necesitaría RTTI o encapsular el Tile/Placeable en una interfaz y expandir en el Solve según la clase
		std::vector<void*>* path = new std::vector<void*>();
		std::vector<void*>* bestPath = new std::vector<void*>();
		float bestCost = FLT_MAX;
		for (auto it = fromTiles.cbegin(); it != fromTiles.cend(); ++it){
			// Reiniciamos
			path->clear();
			float cost = 0.0f;

			Logic::Tile* tileFrom = (*it);

			// Buscamos path
			int solved = _aStarWalkingSoul->Solve((void*)tileFrom, tos, path, &cost);

			// Si se encontró ruta desde el tile actual
			if (solved != micropather::MicroPather::NO_SOLUTION && path->size() > 0) {
				// Si es la primera ruta o mejora la que teníamos hasta el momento
				if (bestPath == nullptr || cost < bestCost){
					// Actualizamos el mejor path copiando el vector
					*bestPath = *path;

					// Actualizamos el mejor coste
					bestCost = cost;
				}
			}
		}

		// Borramos el path auxiliar
		delete path;
		path = nullptr;

		// No se ha encontrado ninguna ruta desde ningún tile
		if (bestPath->size() == 0){
			delete bestPath;
			bestPath = nullptr;
			return nullptr;
		}
		
		// Transformamos el retorno
		std::vector<Vector3>* out = new std::vector<Vector3>();

		// ¿Es necesario añadir la posición actual del Placeable?
		//out->push_back(from->getEntity()->getPosition());
		for (auto it = bestPath->cbegin(); it != bestPath->cend(); it++) {
			Logic::Tile* tile = (Logic::Tile*)(*it);
			out->push_back(tile->getEntity()->getPosition());
		}

		delete bestPath;
		bestPath = nullptr;

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