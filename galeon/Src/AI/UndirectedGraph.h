/**
@file UndirectedGraph.h

Clase que permite crear grafos no dirigidos.

@see UndirectedGraph

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_UndirectedGraph_H
#define __AI_UndirectedGraph_H

#include <vector>
#include <list>
#include <map>

#include "BaseSubsystems/Math.h"

#include "DynTriMat.h"

using namespace std;

/**
Nodo del grafo. Contiene una posici�n.
*/
struct CNode {
	CNode(Vector3 &pos) : position(pos) {};
	Vector3 position;
};

/**
Clase que representa un grafo no dirigido (i.e. las aristas son 
bidireccionales). El grafo est� pensado para ser utilizado como 
grafo de puntos de navegaci�n (waypoint graph). Si se emplea para 
otra cosa ser�a conveniente modificar el dise�o.
<p>
El contenido de los nodos es una posici�n, aunque se puede parametrizar 
cambiando el struct CNode. De la misma manera, las etiquetas contienen 
valores double. Si queremos emplear otro tipo de etiqueta hay que modificar 
la matriz de coste _cost.
*/
class CUndirectedGraph
{
public:
	/**
	Constructor
	*/
	CUndirectedGraph(void);
	/**
	Destructor
	*/
	~CUndirectedGraph(void);
	/**
	A�ade un nodo al grafo. Es el propio grafo en su destructor quien
	se encarga de eliminar los nodos. NO elimina su contenido, solamente
	el CNode.
	
	@param node puntero al nodo que se a�ade.
	*/
	void addNode(CNode* node);
	/**
	Crea una arista entre dos nodos existentes y le asigna un coste.
	
	@param node1 Identificador del primer nodo.
	@param node2 Identificador del segundo nodo.
	@param cost Coste asociado a la arista.
	*/
	void addEdge(unsigned int node1, unsigned int node2, float cost);
	/**
	Devuelve el n�mero de nodos en el grafo.

	@return N�mero de nodos.
	*/
	int getNodeCount() { return _nodes.size(); };
	/**
	Devuelve una lista con todos los nodos conectados al especificado.

	@param idNode Identificador del nodo para el que se va a obtener la 
	lista de vecinos.
	@return Lista de vecinos.
	*/
	const list<unsigned int> &getNeighbours(unsigned int idNode);
	/**
	Devuelve la etiqueta de un nodo.

	@param id Identificador del nodo.
	@return CNode con el contenido del nodo.
	*/
	CNode* getNode(unsigned int id);
	/**
	Devuelve el coste asignado a la arista entre dos nodos.

	@param node1 Un nodo.
	@param node2 Otro nodo.
	@return Coste.
	*/
	float getEdgeCost(unsigned int node1, unsigned int node2) { return _cost.getElement(node1, node2); };

private:
	/** 
	La lista de nodos del grafo. Cada nodo tiene un identificador �nico 
	que vamos a considerar que es la posici�n en este vector, por lo que 
	no se deber�an borrar ni insertar nodos en mitad de la lista.
	<p>
	Es el propio grafo en su destructor quien
	se encarga de eliminar los nodos. NO elimina su contenido, solamente
	el CNode.
	*/
	vector<CNode*> _nodes;
	/**
	Listas de adyacencia de cada nodo. Cada id de nodo est� asociado a una 
	lista con los ids de los nodos adyacentes a �l.	De esta manera es m�s eficiente 
	acceder a todos los vecinos de un nodo que usando una matriz de adyacencia.
	*/
	map<unsigned int, list<unsigned int>*> _adjacency;
	/**
	Matriz con el coste de cada arista del nodo. Est� implementada como una 
	matriz triangular porque el grafo no es dirigido. Se podr�a optimizar m�s
	quitando los elementos de la diagonal si suponemos que no hay aristas que 
	unan un nodo con �l mismo.
	*/
	CDynTriMat<float> _cost;

	/**
	Crea una arista que va desde el nodo de origen (orig) al de destino (dest)
	a�adiendo el segundo a la lista del primero. Crea la lista en el caso de que
	no exista.
	<p>
	@remarks Este m�todo no crea la arista en sentido contrario (desde dest a orig). 
	Hay que hacerlo manualmente.

	@param orig Id del nodo de origen.
	@param dest Id del nodo de destino.
	*/
	void setAdjacent(unsigned int orig, unsigned int dest);
	/**
	N�mero de aristas del grafo
	*/
	unsigned int _edgeCount;
};

#endif __AI_UndirectedGraph_H
