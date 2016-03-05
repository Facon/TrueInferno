#ifndef SM_SOUL_
#define SM_SOUL_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAWaitTaskAndRequestPath.h"
#include "AI\LAWaitAndFollowPath.h"
#include "AI\LAExecuteSoulTask.h"
#include "AI\LADestroyEntity.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"

namespace AI {
	/**
	Esta FSM controla la lógica fundamental de las almas.

	Inicialmente el alma se pone a la espera de que le asignen un objetivo. En cuanto lo tiene solicita cálculo de ruta.
	Cuando le llega la ruta la sigue.
	Tras acabar la ruta ejecuta la tarea que tenía asignada.
	Por último, se autodestruye.
	*/ 
	class CSMSoul : public CStateMachine<CLatentAction, CSMSoulData> {
	public:
		CSMSoul(CEntity* entity) : CStateMachine<CLatentAction, CSMSoulData>(entity) {
			int waitTaskAndRequestPath = this->addNode(new CLAWaitTaskAndRequestPath(entity, _data));
			int waitPathAndFollow = this->addNode(new CLAWaitAndFollowPath(entity, _data));
			int executeTask = this->addNode(new CLAExecuteSoulTask(entity, _data));
			int destroyEntity = this->addNode(new CLADestroyEntity(entity));

			// Inicialmente el alma se pone a la espera de que le asignen un objetivo. En cuanto lo tiene solicita cálculo de ruta
			this->setInitialNode(waitTaskAndRequestPath);

			// Cuando le llega la ruta la sigue
			this->addEdge(waitTaskAndRequestPath, waitPathAndFollow, new CConditionSuccess());
			this->addEdge(waitTaskAndRequestPath, waitTaskAndRequestPath, new CConditionFail());

			// Tras acabar la ruta ejecuta la tarea que tenía asignada
			this->addEdge(waitPathAndFollow, executeTask, new CConditionMessage<CLatentAction,WalkSoulPathMessage>(MessageType::WALK_SOUL_PATH_FINISHED));

			// Y tras ejecutar la destruímos
			this->addEdge(executeTask, destroyEntity, new CConditionSuccess());
			this->addEdge(executeTask, executeTask, new CConditionFail());

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

	private:
		CSMSoulData _data;
	};
}

#endif