/**
@file AStarFunctionsGaleon.cpp

En este fichero se implementan las funciones
necesarias para calcular rutas usando A*.


@author Gonzalo Fl�rez
@date Diciembre, 2010
*/

#include "AStarFunctionsGaleon.h"

#include "Server.h"

namespace AI 
{

	//---------------------------------------------------------
	/** 
	Constructor
	*/
	CAStarFunctionsGaleon::CAStarFunctionsGaleon(void)
	{
	}

	//---------------------------------------------------------
	/** 
	Destructor
	*/
	CAStarFunctionsGaleon::~CAStarFunctionsGaleon(void)
	{
	}

	//---------------------------------------------------------
	/**
	Devuelve el coste seg�n la heur�stica para llegar desde el estado stateStart hasta stateEnd.
	Para que el camino devuelto por A* sea �ptimo la heur�stica sea aceptable y no sobreestimar 
	la distancia.
	Para la b�squeda de caminos de Galeon utilizaremos como heur�stica la distancia eucl�dea
	entre los puntos.
	*/
	float CAStarFunctionsGaleon::LeastCostEstimate( void* stateStart, void* stateEnd )
	{
		// Funci�n heur�stica para A*.
		// En el caso de Gale�n, una heur�stica admisible es la distancia entre los nodos 
		// indicados por stateStart y stateEnd.
		// Para calcularla tenemos que obtener los nodos del grafo de navegaci�n, al que 
		// podemos acceder mediante el servidor de IA
		int idOrigen = (int) stateStart;
		int idDestino = (int) stateEnd;
		// Utilizamos el grafo de navegaci�n:
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		Vector3 orig = wpg->getWaypoint(idOrigen);
		Vector3 dest = wpg->getWaypoint(idDestino);
		return orig.distance(dest);
	}

	//---------------------------------------------------------
	/** 
	Devuelve la lista de vecinos de un nodo junto con el coste de llegar desde el nodo actual
	hasta cada uno de ellos.
	En Galeon usaremos el grafo de waypoints para obtenerla.
	*/	
	void CAStarFunctionsGaleon::AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent )
	{
		// Esta funci�n nos da la lista de vecinos de un nodo y el coste real (no heur�stico) para llegar a
		// cada uno de ellos.
		// Para acceder a la lista de vecinos usamos el grafo de navegaci�n.
		// Para el coste de cada vecino consultamos la arista lo une con el nodo.
		// Estos datos se devuelven en un vector de pares [idNodo, coste] (tipo StateCost).
		int idNodo = (int) state;
		// Utilizamos el grafo de navegaci�n:
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		list<unsigned int> neighbours = wpg->getNeighbours(idNodo);
		// Llenamos la lista de adyacentes con pares de (nodo vecino, coste en llegar)
		for (list<unsigned int>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
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
	void  CAStarFunctionsGaleon::PrintStateInfo( void* state )
	{
		CWaypointGraph* wpg = CServer::getSingletonPtr()->getNavigationGraph();
		Vector3 position = wpg->getNode((int) state)->position;
		printf("(%f, %f, %f)", position.x, position.y, position.z);
	}

	//---------------------------------------------------------

} // namespace AI