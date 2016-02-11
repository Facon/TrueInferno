/**
@file AStarFunctionsWalkingSoul.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas de carreteras usando A*.


@author �lvaro Valera
@date February, 2016
*/

#pragma once

#ifndef AI_STAR_FUNCTIONS_WALKING_SOUL_H_
#define AI_STAR_FUNCTIONS_WALKING_SOUL_H_

#include "micropather.h"

namespace AI 
{
	/**
	Clase que hereda de micropather::Graph y que contiene las funciones
	de A* espec�ficas de Galeon.
	*/
	class CAStarFunctionsWalkingSoul : public micropather::Graph
	{
	public:
		/** 
		Constructor
		*/
		CAStarFunctionsWalkingSoul(void);
		/** 
		Destructor
		*/
		~CAStarFunctionsWalkingSoul(void);
		/**
		Devuelve el coste seg�n la heur�stica para llegar desde el estado stateStart hasta stateEnd.
		Para que el camino devuelto por A* sea �ptimo la heur�stica sea aceptable y no sobreestimar 
		la distancia.
		Para la b�squeda de caminos de almas caminantes en el mapa de Tiles utilizaremos como heur�stica la distancia de Manhattan.
		*/
		virtual float LeastCostEstimate( void* stateStart, void* stateEnd );

		/** 
		Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
		hasta cada uno de ellos.
		*/	
		virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent );

		/**
			This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
			aren't really human readable, normally you print out some concise info (like "(1,2)") 
			without an ending newline.
		*/
		virtual void PrintStateInfo( void* state );

	};

} // namespace AI 

#endif //AI_STAR_FUNCTIONS_WALKING_SOUL_H_