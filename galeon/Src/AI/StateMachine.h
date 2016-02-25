/**
@file CStateMachine.h

En este fichero se define la clase CStateMachine,
que es la clase padre de las m�quinas de estado,
CStateMachineFactory, factor�a de m�quinas de estado
para las pr�cticas, y CSMWander, una m�quina de estado
de ejemplo.

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_StateMachine_H
#define __AI_StateMachine_H


#include "Condition.h"
#include "Logic/Entity/Entity.h"

#include "LatentAction.h"
#include "SimpleLatentActions.h"
#include "LARoutes.h"
#include "LAExecuteSM.h"

using namespace Logic;

namespace AI 
{

	/**
	Clase padre para las m�quinas de estado.
	<p>
	Es una clase parametrizada. El par�metro es la clase de 
	los elementos en los nodos. En general, este par�metro ser�
	una acci�n ejecutable (CLatentAction).
	*/
	template <class TNode>
	class CStateMachine : MessageHandler
	{
	public: 
		/**
		Constructor
		*/
		CStateMachine() : _entity(0), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Constructor que recibe la entidad que ejecuta la m�quina de estado
		*/
		CStateMachine(CEntity* entity) : _entity(entity), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Destructor
		*/
		virtual ~CStateMachine();
		/**
		Este m�todo a�ade un nodo a la m�quina de estado y devuelve un identificador
		del nodo. Este identificador se usa para referirse a los nodos al a�adir
		aristas y establecer el nodo inicial.
		<p>
		Los nodos ser�n destruidos cuando se destruya la m�quina de estado.

		@param content Contenido del nodo.
		@return Identificador para el nodo.
		*/
		int addNode(TNode *content);
		/**
		A�ade una arista a la m�quina de estado.
		<p>
		El m�todo recibe los identificadores del nodo de origen y destino y una condici�n
		que indica cu�ndo se activa la transici�n de la arista.
		<p>
		Las condiciones ser�n destruidas cuando se destruya la m�quina de estado.

		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condici�n asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode> *cond);
		/**
		Este m�todo comprueba las condiciones de las aristas que salen del 
		nodo actual y cambia de nodo si alguna de ellas es cierta. El m�todo
		devuelve true si alguna transici�n es cierta (aunque no se cambie el
		nodo actual) y false en otro caso.
		<p>
		Las aristas se comprueban en el orden en que se han a�adido y 
		la comprobaci�n se detiene en el momento en que una de ellas se hace 
		cierta.
		*/
		bool nextState();
		/**
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode();
		/**
		Establece cu�l es la entidad que ejecuta la m�quina de estado.
		*/
		void setEntity(CEntity *entity) { _entity = entity; };
		/**
		Establece el nodo inicial.

		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { _initialNodeId = idNode; };
		/**
		Reinicia la ejecuci�n de la m�quina de estado.
		*/
		void resetExecution(){ _currentNodeId = -1; };

		//virtual bool accept(const TMessage &message);
		
		//virtual void process(const TMessage &message);

		virtual bool HandleMessage(const WalkSoulPathMessage& msg) = 0;

	protected:
		/**
		Tipo que guarda la informaci�n de las aristas salientes de un nodo.
		Para cada arista tenemos un vector de pares con la condici�n y el 
		nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;
		/** 
		Tipo que guarda la informaci�n de todas las aristas. Est� indexado 
		por el identificador del nodo de origen.
		*/
		typedef std::map<int, PairVector*> EdgeList;
		/**
		Entidad que ejecuta la m�quina de estado.
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
		formada por pares (condici�n, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condici�n es c1 y otra que va desde 1 hasta 3 y su 
		condici�n es c2, la estructura que tendremos ser�:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList * _edges;

	}; // class CStateMachine
	
	/**
	En esta clase implementaremos una m�quina de estado simple que har� que 
	la entidad haga un recorrido a trav�s de varios waypoints.
	Cada nodo de la m�quina de estado almacenar� un int con el n�mero de waypoint
	de destino (3, 4, 5 y 6). Cada nodo est� unido al siguiente con una arista 
	con la condici�n CConditionMessage, que se hace cierta cuando la entidad 
	recibe un mensaje de un tipo determinado. El tipo que usaremos en este caso 
	es FINISHED_ROUTE
	*/
	/*class CSMPatrol : public CStateMachine<int>
	{
	public:
		CSMPatrol(CEntity* entity) : CStateMachine(entity) 
		{
			// TODO PR�CTICA IA
			// Aqu� tiene que venir el c�digo para crear la m�quina de estado:
			// En primer lugar se a�aden los nodos, usando la funci�n addNode.
			// En este caso el contenido de los nodos ser�n waypoints por los 
			// que queremos pasar (3, 4, 5, 6).
			// Esta funci�n devuelve un identificador por cada nodo que 
			// tenemos que guardar para poder crear las aristas.
			int routeTo1 = this->addNode(new int(3));
			int routeTo2 = this->addNode(new int(4));
			int routeTo3 = this->addNode(new int(5));
			int routeTo4 = this->addNode(new int(6));
			// Las aristas se crean usando el m�todo addEdge. En cada arista
			// a�adimos una condici�n del tipo CConditionMessage, con el tipo 
			// de mensaje FINISHED_ROUTE.
			this->addEdge(routeTo1, routeTo2, new CConditionMessage<int>(Message::FINISHED_ROUTE));
			this->addEdge(routeTo2, routeTo3, new CConditionMessage<int>(Message::FINISHED_ROUTE));
			this->addEdge(routeTo3, routeTo4, new CConditionMessage<int>(Message::FINISHED_ROUTE));
			this->addEdge(routeTo4, routeTo1, new CConditionMessage<int>(Message::FINISHED_ROUTE));
			// Por �ltimo hay que decir cu�l es el nodo inicial.
			this->setInitialNode(routeTo1);
			this->resetExecution();
		}
	};*/

	/**
	Esta clase implementa una m�quina de estado para un comportamiento
	Wander. Es un comportamiento simple que consta de dos estados. En primer
	lugar, la entidad ejecuta la acci�n latente CLARouteTo, que busca una ruta 
	hasta un punto aleatorio del mapa. Cuando ha finalizado, ejecuta CLAWait, 
	que espera durante un periodo fijo de tiempo (3 segundos).
	Despu�s de esperar vuelve al comienzo.
	*/
	/*class CSMWander : public CStateMachine<CLatentAction>
	{
	public:
		/**
		Constructor. A�ade los nodos y las aristas, establece el nodo de inicio
		y deja la m�quina de estado lista para ser ejecutada.
		*/
		/*CSMWander(CEntity* entity) : CStateMachine(entity) {

			// TODO PR�CTICA IA
			// Aqu� tiene que venir el c�digo para crear la m�quina de estado:
			// En primer lugar se a�aden los nodos. A continuaci�n, utilizando 
			// los ids que obtenemos al a�adir los nodos se a�aden las aristas.
			// Por �ltimo hay que decir cu�l es el nodo inicial.
			// En este caso los nodos son acciones latentes (CLARouteToRandom 
			// y CLAWait)
			int routeTo = this->addNode(new CLARouteToRandom(entity));
			int wait = this->addNode(new CLAWait(3000));
			this->addEdge(routeTo, wait, new CConditionSuccess());
			this->addEdge(routeTo, routeTo, new CConditionFail());
			this->addEdge(wait, routeTo, new CConditionFinished());
			this->setInitialNode(wait);
			this->resetExecution();
		}
	};

	/**
	Esta clase implementa una m�quina de estado jer�rquica con un comportamiento 
	simple. La m�quina de estado comienza con un nodo compuesto, que contiene 
	una CSMWander. Este nodo se ejecutar� hasta que alcance la posici�n 
	(20, 0, 700) (CConditionNear). Cuando se verifique la condici�n, la entidad
	ir� hasta la posici�n (0, 0, 0) (CLARouteTo). Cuando llegue a esta posici�n,
	el comportamiento vuelve al nodo anterior.
	*/
	/*class CSMHierarchical : public CStateMachine<CLatentAction>
	{
	public:
		CSMHierarchical(CEntity* entity) : CStateMachine(entity)
		{
			// TODO PR�CTICA IA
			// Aqu� tiene que venir el c�digo para crear la m�quina de estado:
			// En primer lugar se a�aden los nodos. A continuaci�n, utilizando 
			// los ids que obtenemos al a�adir los nodos se a�aden las aristas.
			// Por �ltimo hay que decir cu�l es el nodo inicial.

			CStateMachine* smWander = new CSMWander(entity);
			int wander = this->addNode(new CLAExecuteSM(smWander));
			int go_to = this->addNode(new CLARouteTo(entity, Vector3(0, 0, 0)));

			this->addEdge(wander, go_to, new CConditionNear<CLatentAction>(Vector3(-20, 0, 700), 20));
			this->addEdge(go_to, wander, new CConditionFinished());

			this->setInitialNode(wander);
			this->resetExecution();
		}
		
	};*/

	/**
	Factor�a que devuelve m�quinas de estado predefinidas.
	S�lo reconoce el valor "wander" como entrada, 
	que recorre puntos aleatorios del mapa
	*/
	class CStateMachineFactory 
	{
	public:
		static CStateMachine<CLatentAction>* getStateMachine(std::string smName, CEntity * entity)
		{
			/*if (smName == "wander") {
				return new CSMWander(entity);
			} else if (smName == "hfsm") {
				return new CSMHierarchical(entity);
			}*/
			return 0;
		}
	};

//////////////////////////////
//	Implementaci�n de CStateMachine
//////////////////////////////
	template <class TNode>
	CStateMachine<TNode>::~CStateMachine() 
	{
		// Borramos las aristas
		for (EdgeList::iterator it = _edges->begin(); it != _edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++) {
				// Borra la condici�n
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
	template <class TNode>
	int CStateMachine<TNode>::addNode(TNode* content)
	{
		// TODO PR�CTICA IA
		// El nuevo nodo (content) tenemos que a�adirlo a la lista
		// de nodos (_nodes) en la �ltima posici�n
		// Y tenemos que devolver el id, que es la posici�n en la 
		// que lo hemos a�adido
		int id = _nodes.size();
		_nodes[id] = content;
		return id;
	} // addNode
//////////////////////////////
	template <class TNode>
	void CStateMachine<TNode>::addEdge(int idOrig, int idDest, ICondition<TNode> *cond)
	{
		// TODO PR�CTICA IA
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
		// 3. A�adimos la condici�n y el nodo de destino a la lista
		//std::vector<std::pair<ICondition<TNode>*, int>>
		vector->push_back(std::pair<ICondition<TNode>*, int>(cond, idDest));
	} // addEdge
//////////////////////////////
	template <class TNode>
	bool CStateMachine<TNode>::nextState()
	{
		// TODO PR�CTICA IA
		// Si la m�quina no est� inicializada, el nodo actual (_currentNodeId)
		// ser� -1. En ese caso la inicializamos asign�ndole al current el nodo 
		// inicial e indicando que s� que ha habido cambio de nodo
		if (_currentNodeId == -1) {
			_currentNodeId = _initialNodeId;
			return true;
		}
		// En la lista de aristas (_edges) buscamos las que salen del nodo actual
		// (_edges est� indexada por el nodo de origen de las aristas).
		// Si hay alguna arista de salida ==>
		// Recorremos la lista de aristas de salida y para cada una de ellas
		// Comprobamos la condici�n de la arista (m�todo check)
		// Si se verifica actualizamos el nodo actual y devolvemos true
		// Si no hay aristas o no se cumple ninguna condici�n devolvemos false
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			// Sacamos el nodo actual por si hay que comprobar la condici�n
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				TNode* node = _nodes[_currentNodeId];
				// Si la condici�n es verdadera
				if (edgeIt->first->check(node, _entity)) {
					// S�lo hacemos la primera transici�n que encontramos
					int newNode = edgeIt->second;
					_currentNodeId = newNode;
					// Si la transici�n se hace cierta siempre consideramos que el comportamiento cambia
					// Esto implica que si se activa una arista circular (empieza y termina en el mismo nodo)
					// el comportamiento asociado se va a reiniciar
					return true;
				}
			}
		}
		return false;
	} // nextState
//////////////////////////////
	template <class TNode>
	TNode* CStateMachine<TNode>::getCurrentNode()
	{
		return _nodes[_currentNodeId];
	} // getCurrentNode
//////////////////////////////
	/*template <class TNode>
	bool CStateMachine<TNode>::accept(const TMessage &message)
	{
		// Si no hay un nodo actual no hay aristas interesadas
		if (_currentNodeId == -1) 
			return false;
		// Buscamos la lista de aristas que salen del nodo actual
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				// Llamamos al accept de la condici�n
				if (edgeIt->first->accept(message))
					return true;
			}
		}
		return false;
	}*/

//////////////////////////////
	/*template <class TNode>
	void CStateMachine<TNode>::process(const TMessage &message){
		// Si no hay un nodo actual no hay aristas interesadas as� que lo primero es comprobar si hay un nodo v�lido en _currentNodeId
		if (_currentNodeId != -1) { 
			// Buscamos la lista de aristas que salen del nodo actual
			EdgeList::iterator it = _edges->find(_currentNodeId);
			if (it != _edges->end()) {
				PairVector* vector = (*it).second;
				// Para cada elemento del vector (arista que sale del nodo actual)
				for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
					// Llamamos al process de la condici�n
					if (edgeIt->first->accept(message))
						edgeIt->first->process(message);
				}
			}
		}
	}*/

//////////////////////////////

} // namespace AI 

#endif // __AI_StateMachine_H
