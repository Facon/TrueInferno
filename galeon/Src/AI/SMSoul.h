#ifndef SM_SOUL_
#define SM_SOUL_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\LAGetTargetAndRequestPath.h"
#include "AI\LAExecutePath.h"
#include "AI\LAWaitFinishedPath.h"
#include "AI\LAExecuteSoulTask.h"

namespace AI {
	class CSMSoul : public CStateMachine<CLatentAction> {
	public:
		CSMSoul(CEntity* entity) : CStateMachine(entity) {
			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTargetAndRequestPath(entity));
			int executePath = this->addNode(new CLAExecutePath(entity));
			int waitFinishedPath = this->addNode(new CLAWaitFinishedPath(entity));
			int executeTask = this->addNode(new CLAExecuteSoulTask(entity));

			this->addEdge(process, executePath, new CConditionSuccess());
			this->addEdge(process, process, new CConditionFail());

			this->addEdge(executePath, waitFinishedPath, new CConditionSuccess());
			this->addEdge(executePath, process, new CConditionFail());

			this->addEdge(waitFinishedPath, executeTask, new CConditionSuccess());
			this->addEdge(waitFinishedPath, process, new CConditionFail());

			this->addEdge(executeTask, process, new CConditionFinished());

			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMSoul() {}

		bool HandleMessage(const SoulMessage& msg){
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