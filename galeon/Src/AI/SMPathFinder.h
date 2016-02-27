#ifndef SM_PATH_FINDER_
#define SM_PATH_FINDER_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\LAFindingPath.h"

namespace AI {
	class CSMPathFinder : public CStateMachine<CLatentAction> {
	public:
		CSMPathFinder(CEntity* entity) : CStateMachine(entity) {
			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAFindingPath());
			this->addEdge(process, process, new CConditionFinished());

			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMPathFinder() {}

		bool HandleMessage(const WalkSoulPathMessage& msg){
			bool ret = false;

			// Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId
			if (_currentNodeId != -1) {
				// Buscamos la lista de aristas que salen del nodo actual
				EdgeList::iterator it = _edges->find(_currentNodeId);
				if (it != _edges->end()) {
					PairVector* vector = (*it).second;
					// Para cada elemento del vector (arista que sale del nodo actual)
					for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
						// Procesamos en la arista (o sea, la condición)
						ret |= (edgeIt->first->HandleMessage(msg)); // Si alguna arista acepta, aceptaremos al final
					}
				}
			}

			return ret;
		}
	};
}

#endif