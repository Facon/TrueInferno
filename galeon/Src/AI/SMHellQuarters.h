#ifndef SM_HELL_QUARTERS_
#define SM_HELL_QUARTERS_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"

namespace AI {
	class CLAGetTaskAndTarget : public CLatentAction {
	public:
		CLAGetTaskAndTarget() {}
		~CLAGetTaskAndTarget() {}

		virtual bool HandleMessage(const HellQuartersActionMessage& msg)
		{
			if (msg._type != Logic::TMessage::REQUEST_HELLQUARTERS_ACTION)
				return false;

			_walkingSoulTarget = msg.;

			return true;
		}

	protected:
		virtual LAStatus OnRun() {
			// TODO
		}
	};

	class CSMHellQuarters : public CStateMachine<CLatentAction> {
	public:
		CSMHellQuarters(CEntity* entity) : CStateMachine(entity) {
			/** Estado inactivo. Espera tiempo infinito */
			int idle = this->addNode(new CLAWait());

			/** Estado de espera de petici�n. Se espera una cantidad razonable finita de tiempo */
			int waitingRequest = this->addNode(new CLAWait(TIMEOUT));

			/** Estado de obtenci�n de tarea y objetivo */
			int process = this->addNode(new CLAGetTaskAndTarget());

			/** Pasamos de inactivo a esperando petici�n cuando nos despiertan */
			this->addEdge(idle, waitingRequest, new CConditionMessage<CLatentAction>(MessageType::WAKE_UP));

			/** Si hay �xito y se recibe petici�n pasamos de espera a procesando. Si no recibimos nada volvemos a inactivo */
			this->addEdge(waitingRequest, process, new CConditionSuccess());
			this->addEdge(waitingRequest, idle, new CConditionFail());

			/** Una vez procesada la petici�n al estado inactivo */
			this->addEdge(process, idle, new CConditionFinished());
			
			this->setInitialNode(idle);
			this->resetExecution();
		}

		virtual ~CSMHellQuarters() {}

		bool HandleMessage(const HellQuartersActionMessage& msg){
			bool ret = false;

			// Si no hay un nodo actual no hay aristas interesadas as� que lo primero es comprobar si hay un nodo v�lido en _currentNodeId
			if (_currentNodeId != -1) {
				// Buscamos la lista de aristas que salen del nodo actual
				EdgeList::iterator it = _edges->find(_currentNodeId);
				if (it != _edges->end()) {
					PairVector* vector = (*it).second;
					// Para cada elemento del vector (arista que sale del nodo actual)
					for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
						// Procesamos en la arista (o sea, la condici�n)
						ret |= (edgeIt->first->HandleMessage(msg)); // Si alguna arista acepta, aceptaremos al final
					}
				}
			}

			return ret;
		}

	private:
		/** M�ximo tiempo (ms) que se espera antes de volver al estado inactivo si no se recibe la petici�n de ruta */
		const int TIMEOUT = 2000;
	};
}

#endif