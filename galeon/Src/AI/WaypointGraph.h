/**
@file WaypointGraph.h

Clase para mantener grafos de puntos de ruta (waypoints).

@see WaypointGraph

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_WaypointGraph_H
#define __AI_WaypointGraph_H

#include "BaseSubsystems/Math.h"

#include "UndirectedGraph.h"

namespace AI {

/**
Esta clase mantiene un grafo con los puntos de ruta. Cada aristas 
del grafo representa que existe un camino entre los nodos que une 
y la etiqueta de la arista, la distancia.
<p>
El grafo es no dirigido, lo que significa que no se pueden representar 
aristas unidireccionales (por ejemplo, precipicios donde un personaje 
puede caer pero no puede volver a subir).
<p>
La generaci�n del grafo es autom�tica. En primer lugar se a�aden todos 
los nodos con el m�todo addWaypoint y, a continuaci�n, se invoca a 
computeNavigationGraph para generar el grafo. Mientras, se genera se 
lleva a cabo una comprobaci�n para averiguar si el grafo obtenido es 
conexo. 
*/
class CWaypointGraph
{
public:
	/**
	Constructor
	*/
	CWaypointGraph(void);
	/**
	Destructor
	*/
	~CWaypointGraph(void);
	/**
	A�ade un nuevo waypoint al grafo.
	*/
	void addWaypoint(Vector3 waypoint);
	/**
	Calcula las aristas entre los waypoints. Para ello, entre cada par 
	de waypoints hace una comprobaci�n de visibilidad. Si un waypoint es
	visible desde otro, se a�ade una arista entre ellos.
	*/
	void computeNavigationGraph();
	/**
	Devuelve true si se han calculado las aristas y no se ha a�adido despu�s
	ning�n nodo.
	*/
	bool isClean();
	/** 
	Devuelve true si el grafo es conexo (existe un camino entre cualquier 
	par de nodos).
	*/
	bool isConnected();
	/**
	Devuelve un nodo del grafo.
	*/
	const CNode* getNode(int idNode) { return _g.getNode(idNode); } ;
	/**
	Devuelve el coste asociado a la arista entre dos nodos (la distancia).
	*/
	float getCost(int node1, int node2) { return _g.getEdgeCost(node1, node2); };
	/**
	Devuelve la lista de vecinos de un nodo.
	*/
	list<unsigned int> getNeighbours(int idNode) { return _g.getNeighbours(idNode); };
	/**
	Obtiene el waypoint m�s cercano a una posici�n dada.
	<p>
	Hace una b�squeda lineal en la lista de nodos y devuelve el m�s cercano.
	*/
	int getClosestWaypoint(Vector3 position);
	/**
	Devuelve el n�mero de waypoints que contiene el grafo.
	*/
	int getWaypointCount() { return _g.getNodeCount(); };
	/**
	Devuelve la posici�n de un waypoint, dado su identificador.
	*/
	Vector3 getWaypoint(int numWaypoint) { return _g.getNode(numWaypoint)->position; };
		
	/** 
 	Cambia la altura de todos los waypoints del grafo

	@param height Nueva altura
	*/	
	void setWaypointHeight(float height);


private:
	/**
	Ser� cierto si el grafo se ha generado al menos 1 vez
	*/
	bool _generated;
	/** 
	Es cierto si el grafo generado es conexo. Es falso si no es conexo o
	a�n no se ha generado.
	*/
	bool _connected;
	/** 
	Es cierto si se ha generado y no se ha a�adido despu�s ning�n nodo.
	Es falso si se ha a�adido alg�n nodo o no se ha generado.
	*/
	bool _clean;
	CUndirectedGraph _g;

	// TEST
	bool getVisible(int id1, int id2);

}; // class CWaypointGraph

} // namespace AI

#endif __AI_WaypointGraph_H
