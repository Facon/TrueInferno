/**
@file AStarFunctionsSoulPath.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas de almas caminantes usando A*.


@author Álvaro Valera
@date February, 2016
*/

#include "AStarFunctionsSoulPath.h"
#include "Server.h"

#include "Logic\Entity\Components\Tile.h"

#include "OgreVector3.h"

namespace AI
{

	//---------------------------------------------------------
	/**
	Constructor
	*/
	CAStarFunctionsSoulPath::CAStarFunctionsSoulPath(void)
	{
	}

	//---------------------------------------------------------
	/**
	Destructor
	*/
	CAStarFunctionsSoulPath::~CAStarFunctionsSoulPath(void)
	{
	}

	//---------------------------------------------------------
	/**
	Devuelve el coste según la heurística para llegar desde el estado stateStart hasta stateEnd.
	Para que el camino devuelto por A* sea óptimo la heurística sea aceptable y no sobreestimar
	la distancia.
	Para la búsqueda de rutas de SoulPaths en el mapa de Tiles utilizaremos como heurística la distancia de Manhattan.
	*/
	float CAStarFunctionsSoulPath::LeastCostEstimate(void* stateStart, void* stateEnd)
	{
		// Función heurística para A*.
		// En el caso de Galeón, una heurística admisible es la distancia entre los nodos 
		// indicados por stateStart y stateEnd.
		Logic::Tile* tileFrom = (Logic::Tile*)stateStart;
		Logic::Tile* tileTo = (Logic::Tile*)stateEnd;

		return tileTo->manhattanDistance(*tileFrom);
	}

	//---------------------------------------------------------
	/**
	Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
	hasta cada uno de ellos.
	*/
	void CAStarFunctionsSoulPath::AdjacentCost(void* state, std::vector< micropather::StateCost > *adjacent)
	{
		Logic::Tile* current = (Logic::Tile*) state;

		// Si la tile no permite SoulPaths quizás no deberíamos ni haber llegado aquí
		if (!current->canPassSoulPath())
			return;

		// Si las permite, chequeamos sus tiles adyacentes
		for (auto it = current->getAdjacentTiles()->cbegin(); it != current->getAdjacentTiles()->cend(); ++it) {
			// Si la tile adyacente permite SoulPaths la añadimos a la lista de adyacencia con mismo coste
			if ((*it)->canPassSoulPath()){
				micropather::StateCost nodeCost = { (void*)(*it), 1.0f };
				adjacent->push_back(nodeCost);
			}
		}
	}

	//---------------------------------------------------------
	/**
		This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
		aren't really human readable, normally you print out some concise info (like "(1,2)") 
		without an ending newline.
	*/
	void CAStarFunctionsSoulPath::PrintStateInfo( void* state )
	{
		Logic::Tile *tile = (Logic::Tile*)state;
		printf("(%f, %f, %f)", tile->getLogicPosition().x, tile->getLogicPosition().y, tile->getLogicPosition().z);
	}

	//---------------------------------------------------------

} // namespace AI