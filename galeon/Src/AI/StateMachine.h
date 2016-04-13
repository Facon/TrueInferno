/**
@file CStateMachine.h

En este fichero se define la clase CStateMachine,
que es la clase padre de las máquinas de estado,
CStateMachineFactory, factoría de máquinas de estado
para las prácticas, y CSMWander, una máquina de estado
de ejemplo.

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_StateMachine_H
#define __AI_StateMachine_H

#include "Condition.h"
#include "Logic/Entity/Entity.h"

#include "LatentAction.h"
#include "SimpleLatentActions.h"
#include "LAExecuteSM.h"

#include <string>

/** Flag para activar el log para el debug de las SM.
* Adicionalmente las SMs que se quieran analizar deben crearse con _debug a true */
//#define DEBUG_SM

/** Macro para manejar mensajes en una SM (i.e. hijos de StateMachine)
* Propaga el mensaje adecuadamente en una SM para que llegue a las condiciones de transición entre estados.
*/
#define SM_HANDLE_MESSAGE(Class) \
bool HandleMessage(const Class& msg){ \
	bool ret = false; \
\
	/* Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId */ \
	if (_currentNodeId != -1) {  \
		/* Buscamos la lista de aristas que salen del nodo actual */ \
		EdgeList::iterator it = _edges->find(_currentNodeId); \
		if (it != _edges->end()) { \
			PairVector* vector = (*it).second; \
\
			/* Para cada elemento del vector (arista que sale del nodo actual) */ \
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){ \
				/* Procesamos en la arista (o sea, la condición) */ \
				ret |= (edgeIt->first->HandleMessage(msg)); /* Si alguna arista acepta, aceptaremos al final */ \
			} \
		}  \
	} \
\
	return ret; \
}

/** Macro para manejar mensajes en una SM (i.e. hijos de StateMachine)
* Permite procesar globalmente el mensaje en la SM con el método SMHandleMessage y propaga el mensaje
* adecuadamente para que llegue a las condiciones de transición entre estados
*/
#define SM_HANDLE_MESSAGE_WGLOBAL(Class) \
bool HandleMessage(const Class& msg){ \
	bool ret = false; \
\
	ret |= SMGlobalHandleMessage(msg); \
\
	/* Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId */ \
	if (_currentNodeId != -1) {  \
		/* Buscamos la lista de aristas que salen del nodo actual */ \
		EdgeList::iterator it = _edges->find(_currentNodeId); \
		if (it != _edges->end()) { \
			PairVector* vector = (*it).second; \
\
			/* Para cada elemento del vector (arista que sale del nodo actual) */ \
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){ \
				/* Procesamos en la arista (o sea, la condición) */ \
				ret |= (edgeIt->first->HandleMessage(msg)); /* Si alguna arista acepta, aceptaremos al final */ \
						} \
				}  \
		} \
\
	return ret; \
}

using namespace Logic;

namespace AI 
{

	/**
	Clase padre para las máquinas de estado.
	<p>
	Es una clase parametrizada. El primer parámetro es la clase de 
	los elementos en los nodos. En general, este parámetro será
	una acción ejecutable (CLatentAction).
	El segundo parámetro es la clase que contendrá los datos compartidos
	entre los estados de la SM
	*/
	template <class TNode, class SharedData>
	class CStateMachine : public MessageHandler
	{
	public: 
		/**
		Constructor
		*/
		CStateMachine() : _entity(0), _currentNodeId(-1), _initialNodeId(-1), _name(""), _debug(false) { _edges = new EdgeList(); };
		
		/**
		Constructor que recibe la entidad que ejecuta la máquina de estado
		*/
		CStateMachine(CEntity* entity) : _entity(entity), _currentNodeId(-1), _initialNodeId(-1), _name(""), _debug(false) { _edges = new EdgeList(); };
		
		/**
		Destructor
		*/
		virtual ~CStateMachine();
		
		/** Spawnea la máquina de estados. Se le pasa la entidad de mapa para que obtenga de ahí los atributos necesarios */
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) = 0;

		/**
		Este método añade un nodo a la máquina de estado y devuelve un identificador
		del nodo. Este identificador se usa para referirse a los nodos al añadir
		aristas y establecer el nodo inicial.
		<p>
		Los nodos serán destruidos cuando se destruya la máquina de estado.

		@param content Contenido del nodo.
		@param data Datos compartidos de la SM
		@return Identificador para el nodo.
		*/
		int addNode(TNode* content);
		
		/**
		Añade una arista a la máquina de estado.
		<p>
		El método recibe los identificadores del nodo de origen y destino y una condición
		que indica cuándo se activa la transición de la arista.
		<p>
		Las condiciones serán destruidas cuando se destruya la máquina de estado.

		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condición asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode>* cond);
		
		/**
		Este método comprueba las condiciones de las aristas que salen del 
		nodo actual y cambia de nodo si alguna de ellas es cierta. El método
		devuelve true si alguna transición es cierta (aunque no se cambie el
		nodo actual) y false en otro caso.
		<p>
		Las aristas se comprueban en el orden en que se han añadido y 
		la comprobación se detiene en el momento en que una de ellas se hace 
		cierta.
		*/
		bool nextState();
		
		/**
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode();
		
		/**
		Establece cuál es la entidad que ejecuta la máquina de estado.
		*/
		void setEntity(CEntity *entity) { _entity = entity; };
		
		/**
		Establece el nodo inicial.

		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { _initialNodeId = idNode; };
		
		/**
		Reinicia la ejecución de la máquina de estado.
		*/
		void resetExecution(){ _currentNodeId = -1; };

		/**
		Desactiva la máquina de estados. Se invoca automáticamente al desactivar el componente SMExecutor
		*/
		virtual void deactivate() {}

		/**
		Devuelve el objeto de datos compartido de la máquina de estados
		*/
		SharedData getSharedData(){
			return _data;
		}

	protected:
		/**
		Tipo que guarda la información de las aristas salientes de un nodo.
		Para cada arista tenemos un vector de pares con la condición y el 
		nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;

		/**
		Estructura de datos compartida entre los estados de la SM 
		*/
		SharedData _data;

		/** 
		Tipo que guarda la información de todas las aristas. Está indexado 
		por el identificador del nodo de origen.
		*/
		typedef std::map<int, PairVector*> EdgeList;

		/**
		Entidad que ejecuta la máquina de estado.
		*/
		Logic::CEntity *_entity;

		/**
		Valores del nodo actual e inicial
		*/
		int _currentNodeId, _initialNodeId;

		/**
		Lista de nodos. Es un map que relaciona cada identificador de nodo con su contenido.
		*/
		std::map<int, TNode*> _nodes;

		/**
		Lista de aristas. Es un map que asocia cada nodo de origen de cada arista con una lista
		formada por pares (condición, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condición es c1 y otra que va desde 1 hasta 3 y su 
		condición es c2, la estructura que tendremos será:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList * _edges;

		// Nombre de la SM
		std::string _name;

		bool _debug;

	}; // class CStateMachine
	
	/**
	Factoría que devuelve máquinas de estado predefinidas.
	Sólo reconoce el valor "wander" como entrada, 
	que recorre puntos aleatorios del mapa
	*/
	/*class CStateMachineFactory 
	{
	public:
		static CStateMachine<CLatentAction>* getStateMachine(std::string smName, CEntity * entity)
		{
			if (smName == "wander") {
				return new CSMWander(entity);
			} else if (smName == "hfsm") {
				return new CSMHierarchical(entity);
			}
			return 0;
		}
	};*/

//////////////////////////////
//	Implementación de CStateMachine
//////////////////////////////
	template <class TNode, class SharedData>
	CStateMachine<TNode, SharedData>::~CStateMachine()
	{
		// Borramos las aristas
		for (EdgeList::iterator it = _edges->begin(); it != _edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++) {
				// Borra la condición
				delete it2->first;
			}
			// Borra el vector
			delete v;
		}
		delete _edges;

		// Borramos los nodos
		for (std::map<int, TNode*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
		{
			delete it->second;
		}
	}

	//////////////////////////////

	template <class TNode, class SharedData>
	int CStateMachine<TNode, SharedData>::addNode(TNode* content)
	{
		// El nuevo nodo (content) tenemos que añadirlo a la lista
		// de nodos (_nodes) en la última posición
		// Y tenemos que devolver el id, que es la posición en la 
		// que lo hemos añadido
		int id = _nodes.size();
		_nodes[id] = content;
		return id;
	} // addNode

	//////////////////////////////
	
	template <class TNode, class SharedData>
	void CStateMachine<TNode, SharedData>::addEdge(int idOrig, int idDest, ICondition<TNode> *cond)
	{
		// 1. Buscamos en la lista de aristas (_edges) las que salen de idOrig
		// (_edges es un map indexado por el origen de cada arista)
		EdgeList::iterator it = _edges->find(idOrig);
		// 2.1 Si no hay ninguna arista saliente para ese nodo tendremos
		// que crear un nuevo PairVector (que es la estructura donde se almacenan
		// las aristas salientes) y asignarla.
		PairVector* vector = NULL;
		if (it == _edges->end()) {
			vector = new PairVector();
			(*_edges)[idOrig] = vector;
		} else {
			// 2.2 Si ya hay una lista la sacamos
			vector = (*it).second;
		}
		// 3. Añadimos la condición y el nodo de destino a la lista
		//std::vector<std::pair<ICondition<TNode>*, int>>
		vector->push_back(std::pair<ICondition<TNode>*, int>(cond, idDest));
	} // addEdge

	//////////////////////////////

	template <class TNode, class SharedData>
	bool CStateMachine<TNode, SharedData>::nextState()
	{
		// Si la máquina no está inicializada, el nodo actual (_currentNodeId)
		// será -1. En ese caso la inicializamos asignándole al current el nodo 
		// inicial e indicando que sí que ha habido cambio de nodo
		if (_currentNodeId == -1) {
			_currentNodeId = _initialNodeId;
			return true;
		}
		// En la lista de aristas (_edges) buscamos las que salen del nodo actual
		// (_edges está indexada por el nodo de origen de las aristas).
		// Si hay alguna arista de salida ==>
		// Recorremos la lista de aristas de salida y para cada una de ellas
		// Comprobamos la condición de la arista (método check)
		// Si se verifica actualizamos el nodo actual y devolvemos true
		// Si no hay aristas o no se cumple ninguna condición devolvemos false
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			// Sacamos el nodo actual por si hay que comprobar la condición
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				TNode* node = _nodes[_currentNodeId];
				// Si la condición es verdadera
				if (edgeIt->first->check(node, _entity)) {
					// Sólo hacemos la primera transición que encontramos
					int newNode = edgeIt->second;

#ifdef DEBUG_SM
					if (_debug){
						//std::cout << _entity->getEntityID() << "," << _name << "," << _nodes[_currentNodeId]->getName() << "," << _nodes[newNode]->getName() << std::endl;
						//std::cout << _entity->getEntityID() << "," << _name << "," << _currentNodeId << "," << newNode << std::endl;
						std::cout << _entity->getEntityID() << "," << _name << "," << _nodes[_currentNodeId]->GetRTTI().GetClassName() << "," << _nodes[newNode]->GetRTTI().GetClassName() << std::endl;
					}
#endif

					_currentNodeId = newNode;
					// Si la transición se hace cierta siempre consideramos que el comportamiento cambia
					// Esto implica que si se activa una arista circular (empieza y termina en el mismo nodo)
					// el comportamiento asociado se va a reiniciar
					return true;
				}
			}
		}
		return false;
	} // nextState

	//////////////////////////////

	template <class TNode, class SharedData>
	TNode* CStateMachine<TNode, SharedData>::getCurrentNode()
	{
		return _nodes[_currentNodeId];
	} // getCurrentNode

} // namespace AI 

#endif // __AI_StateMachine_H
