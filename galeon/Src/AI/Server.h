/**
@file Server.h

Servidor de IA.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

#include "BaseSubsystems\Math.h"

#include "AStarFunctionsWalkingSoul.h"
#include "AStarFunctionsSoulPath.h"
#include "micropather.h"

#include <unordered_set>
#include <vector>

namespace Logic {
	class Tile;
	class CPlaceable;
	class CTileManager;
}

namespace AI {


/**
Servidor de IA. De momento s�lo se encarga de mantener el
grafo con los waypoints y de centralizar las llamadas a A*.

La idea es que en un futuro tambi�n se ocupe de gestionar la 
percepci�n.
*/
class CServer
{
	public:

		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor de IA.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor de IA. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor de IA. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n invocada en cada vuelta para actualizaci�n.

		@param secs Segundos desde que se renderiz� el �ltimo frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(float secs);

		/**
		Calcula con A* una ruta de soulpaths desde un cierto Tile a otro.
		*/
		std::vector<Logic::Tile*>* getSoulPathAStarRoute(Logic::Tile* from, Logic::Tile* to);

		/** Calcula con A* una ruta para almas caminantes desde un punto hasta un placeable */
		std::vector<Vector3>* getWalkingSoulAStarRoute(const Vector3& from, Logic::CPlaceable* to);

		/** Calcula con A* una ruta para almas caminantes desde un placeable a otro */
		std::vector<Vector3>* getWalkingSoulAStarRoute(Logic::CPlaceable* from, Logic::CPlaceable* to);

		/** Calcula con A* una ruta para almas caminantes desde un conjunto de puntos a otro */
		std::vector<Vector3>* getWalkingSoulAStarRoute(const std::vector<Vector3>& from, const std::vector<Vector3>& to);

		/** Calcula con A* una ruta para almas caminantes desde un conjunto de tiles a otro */
		std::vector<Vector3>* getWalkingSoulAStarRoute(std::unordered_set<Logic::Tile*> fromTiles, std::unordered_set<Logic::Tile*> toTiles);

		/**
		Dado un �ngulo en radianes lo lleva al intervalo [-PI, PI]
		*/
		static double correctAngle(double angle);

	private:
		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();
		/**
		Instancia �nica de la clase.
		*/
		static CServer *_instance;

		/** 
		Funciones de distancia para calcular A* en los problemas de obtenci�n de rutas de SoulPaths
		*/
		CAStarFunctionsSoulPath* _fSoulPath;

		/**
		Funciones de distancia para calcular A* en los problemas de obtenci�n de rutas para las almas caminantes
		*/
		CAStarFunctionsWalkingSoul* _fWalkingSoul;

		/** Micropather especializado para las almas caminantes */
		micropather::MicroPather* _aStarWalkingSoul;

		/** Micropather especializado para las SoulPaths */
		micropather::MicroPather* _aStarSoulPath;

		Logic::CTileManager* _tileManager;
}; // class CServer

} // namespace AI
#endif