/**
@file AStarFunctionsRoadPath.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas de carreteras usando A*.


@author Álvaro Valera
@date February, 2016
*/

#include "AStarFunctionsRoadPath.h"

#include "Server.h"
#include "Logic\Entity\Components\Tile.h"

namespace AI 
{

	//---------------------------------------------------------
	/** 
	Constructor
	*/
	CAStarFunctionsRoadPath::CAStarFunctionsRoadPath(void)
	{
	}

	//---------------------------------------------------------
	/** 
	Destructor
	*/
	CAStarFunctionsRoadPath::~CAStarFunctionsRoadPath(void)
	{
	}

	//---------------------------------------------------------
	/**
	Devuelve el coste según la heurística para llegar desde el estado stateStart hasta stateEnd.
	Para que el camino devuelto por A* sea óptimo la heurística sea aceptable y no sobreestimar 
	la distancia.
	Para la búsqueda de caminos de carretera en el mapa de Tiles utilizaremos como heurística la distancia de Manhattan.
	*/
	float CAStarFunctionsRoadPath::LeastCostEstimate( void* stateStart, void* stateEnd )
	{
		// Función heurística para A*.
		// En el caso de Galeón, una heurística admisible es la distancia entre los nodos 
		// indicados por stateStart y stateEnd.
		Logic::Tile* tileFrom = (Logic::Tile*)stateStart;
		Logic::Tile* tileTo = (Logic::Tile*)stateEnd;

		return tileFrom->getLogicPosition().manhattanDistance(tileTo->getLogicPosition());
	}

	//---------------------------------------------------------
	/** 
	Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
	hasta cada uno de ellos.
	*/	
	void CAStarFunctionsRoadPath::AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent )
	{
		// Esta función nos da la lista de vecinos de un nodo y el coste real (no heurístico) para llegar a
		// cada uno de ellos.
		Logic::Tile* current = (Logic::Tile*) state;
		
		if (current->canBuildRoad())

		// Llenamos la lista de adyacentes con pares de (nodo vecino, coste en llegar)
		for (auto it = current->getAdjacentTiles().cbegin(); it != current->getAdjacentTiles().cbegin(); ++it) {
			Logic::Tile* adjacent = (Logic::Tile*)(*it);
			

			micropather::StateCost nodeCost = {(void*)(*it), wpg->getCost(idNodo, (*it))};
			adjacent->push_back(nodeCost);
		}
	}

	//---------------------------------------------------------
	/**
		This function is only used in DEBUG mode - it dumps output to stdout. Since void* 
		aren't really human readable, normally you print out some concise info (like "(1,2)") 
		without an ending newline.
	*/
	void  CAStarFunctionsRoadPath::PrintStateInfo( void* state )
	{
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		Vector3 position = wpg->getNode((int) state)->position;
		printf("(%f, %f, %f)", position.x, position.y, position.z);
	}

	//---------------------------------------------------------

} // namespace AI