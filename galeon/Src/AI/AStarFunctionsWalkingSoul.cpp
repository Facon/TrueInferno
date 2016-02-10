/**
@file AStarFunctionsRoadPath.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas de soulpaths usando A*.


@author �lvaro Valera
@date February, 2016
*/

#include "AStarFunctionsWalkingSoul.h"
#include "Server.h"

#include "Logic\Entity\Components\Tile.h"

#include "OgreVector3.h"

namespace AI 
{

	//---------------------------------------------------------
	/** 
	Constructor
	*/
	CAStarFunctionsWalkingSoul::CAStarFunctionsWalkingSoul(void)
	{
	}

	//---------------------------------------------------------
	/** 
	Destructor
	*/
	CAStarFunctionsWalkingSoul::~CAStarFunctionsWalkingSoul(void)
	{
	}

	//---------------------------------------------------------
	/**
	Devuelve el coste seg�n la heur�stica para llegar desde el estado stateStart hasta stateEnd.
	Para que el camino devuelto por A* sea �ptimo la heur�stica sea aceptable y no sobreestimar
	la distancia.
	Para la b�squeda de caminos de almas caminantes en el mapa de Tiles utilizaremos como heur�stica la distancia de Manhattan.
	*/
	float CAStarFunctionsWalkingSoul::LeastCostEstimate(void* stateStart, void* stateEnd)
	{
		// Funci�n heur�stica para A*.
		// En el caso de Gale�n, una heur�stica admisible es la distancia entre los nodos 
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
	void CAStarFunctionsWalkingSoul::AdjacentCost(void* state, std::vector< micropather::StateCost > *adjacent)
	{
		Logic::Tile* current = (Logic::Tile*) state;

		// Si la tile no permite almas caminantes quiz�s no deber�amos ni haber llegado aqu�
		if (!current->canPassWalkingSoul())
			return;

		// Si las permite, chequeamos sus tiles adyacentes
		for (auto it = current->getAdjacentTiles().cbegin(); it != current->getAdjacentTiles().cbegin(); ++it) {
			// Si la tile adyacente permite almas caminantes la a�adimos a la lista de adyacencia con mismo coste
			if ((*it)->canPassWalkingSoul()){
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
	void CAStarFunctionsWalkingSoul::PrintStateInfo(void* state)
	{
		Logic::Tile *tile = (Logic::Tile*)state;
		printf("(%f, %f, %f)", tile->getLogicPosition().x, tile->getLogicPosition().y, tile->getLogicPosition().z);
	}

	//---------------------------------------------------------

} // namespace AI