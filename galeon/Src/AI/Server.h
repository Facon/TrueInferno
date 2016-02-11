/**
@file Server.h

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

#include "AStarFunctionsWalkingSoul.h"
#include "AStarFunctionsSoulPath.h"
#include "micropather.h"

namespace Logic {
	class Tile;
	class CTileManager;
}

#include <vector>

namespace AI {


/**
Servidor de IA. De momento sólo se encarga de mantener el
grafo con los waypoints y de centralizar las llamadas a A*.

La idea es que en un futuro también se ocupe de gestionar la 
percepción.
*/
class CServer
{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor de IA.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se renderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(float secs);

		/**
		Calcula una ruta de soulpaths usando A* desde un cierto tile a otro.
		*/
		std::vector<Logic::Tile*> *getSoulPathAStarRoute(Logic::Tile* from, Logic::Tile* to);

		/**
		Calcula una ruta para almas caminantes usando A* desde un cierto tile a otro.
		*/
		std::vector<Logic::Tile*> *getWalkingSoulAStarRoute(Logic::Tile* from, Logic::Tile* to);

		/**
		Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
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
		Instancia única de la clase.
		*/
		static CServer *_instance;

		/** 
		Funciones de distancia para calcular A* en los problemas de obtención de rutas de SoulPaths
		*/
		CAStarFunctionsSoulPath* _fSoulPath;

		/**
		Funciones de distancia para calcular A* en los problemas de obtención de rutas para las almas caminantes
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