/**
@file Server.h

Servidor de IA.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

#include "WaypointGraph.h"
#include "AStarFunctionsGaleon.h"
#include "micropather.h"

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
		Calcula una ruta usando A*.
		*/
		vector<Vector3> *getAStarRoute(Vector3 from, Vector3 to);

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
		Clase que se encarga de calcular la mejor ruta con A*
		*/
		micropather::MicroPather* _aStar;

		/** 
		Funciones de distancia para calcular A* en los problemas de obtenci�n de rutas de carretera
		*/
		CAStarFunctionsRoadPath* _f;

		/**
		Funciones de distancia para calcular A* en los problemas de obtenci�n de rutas para las almas
		*/
		CAStarFunctionsSoulPath* _f;
}; // class CServer

} // namespace AI
#endif