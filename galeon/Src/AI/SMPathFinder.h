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
			/** Estado inactivo. Espera tiempo infinito */
			int idle = this->addNode(new CLAWait());

			/** Estado de espera de petición. Se espera una cantidad razonable finita de tiempo */
			int waitingRequest = this->addNode(new CLAWait(TIMEOUT));

			/** Estado de cálculo de ruta */
			int process = this->addNode(new CLAFindingPath());

			/** Pasamos de inactivo a esperando petición cuando nos despiertan */
			this->addEdge(idle, waitingRequest, new CConditionMessage<CLatentAction>(MessageType::WAKE_UP));

			/** Si hay éxito y se recibe petición pasamos de espera a buscando ruta. Si no recibimos nada volvemos a inactivo */
			this->addEdge(waitingRequest, process, new CConditionSuccess());
			this->addEdge(waitingRequest, idle, new CConditionFail());

			/** Una vez calculada y devuelta la ruta volvemos al estado inactivo */
			this->addEdge(process, idle, new CConditionFinished());
			
			this->setInitialNode(idle);
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

	private:
		/** Máximo tiempo (ms) que se espera antes de volver al estado inactivo si no se recibe la petición de ruta */
		const int TIMEOUT = 2000;
	};
}

#endif