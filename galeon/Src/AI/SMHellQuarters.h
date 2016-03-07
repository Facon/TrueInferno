#ifndef SM_HELL_QUARTERS_
#define SM_HELL_QUARTERS_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "LAGetTaskAndTarget.h"

namespace AI {
	class CSMHellQuartersData {
	public:
		CSMHellQuartersData() {}
		virtual ~CSMHellQuartersData() {}
	};

	class CSMHellQuarters : public CStateMachine<CLatentAction, CSMHellQuartersData> {
	public:
		CSMHellQuarters(CEntity* entity) : CStateMachine(entity) {
			CSMHellQuartersData data;

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTaskAndTarget(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMHellQuarters() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		bool HandleMessage(const HellQuartersMessage& msg){
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