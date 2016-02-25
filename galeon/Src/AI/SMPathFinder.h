#ifndef SM_PATH_FINDER_
#define SM_PATH_FINDER_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"

namespace AI {
	template <class TNode>
	class CSMPathFinder : public CStateMachine<TNode> {
	public:
		CSMPathFinder(CEntity* entity) : CStateMachine(entity) {
			/** Estado inactivo. Espera tiempo infinito */
			int idle = this->addNode(new CLAWait());

			/** Estado de espera de petición. Se espera una cantidad razonable finita de tiempo */
			int waitingRequest = this->addNode(new CLAWait(TIMEOUT));

			/** Estado de cálculo de ruta */
			int findingPath = this->addNode(new CLAFindingPath());

			/** Pasamos de inactivo a esperando petición cuando nos despiertan */
			this->addEdge(idle, waitingRequest, new CConditionMessage<TNode>(MessageType::PATH_FINDER_WAKE_UP));

			/** Si hay éxito y se recibe petición pasamos de espera a buscando ruta. Si no recibimos nada volvemos a inactivo */
			this->addEdge(waitingRequest, findingPath, new CConditionSuccess());
			this->addEdge(waitingRequest, idle, new CConditionFail());

			/** Una vez calculada y devuelta la ruta volvemos al estado inactivo */
			this->addEdge(findingPath, idle, new CConditionFinished());
			
			this->setInitialNode(idle);
			this->resetExecution();
		}

		~CSMPathFinder() {}

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
		const TIMEOUT = 2000;
	};

	class CLAFindingPath : public CLatentAction{
	public:
		CLAFindingPath() {}
		~CLAFindingPath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg)
		{
			if (msg._type != Logic::TMessage::REQUEST_WALK_SOUL_PATH)
				return false;

			_walkingSoulTarget = msg._target;

			return true;
		}

	protected:
		virtual LAStatus OnRun() {
			// Calculamos ruta desde la posición actual de la entidad hasta el objetivo que nos han dado
			std::vector<Vector3>* path = AI::CServer::getSingletonPtr()->getWalkingSoulAStarRoute(_entity->getPosition(), _walkingSoulTarget);

			// Fallamos si no hay ruta
			if (path == nullptr)
				return LAStatus::FAIL;

			WalkSoulPathMessage message(path);
			message._type = MessageType::RETURN_WALK_SOUL_PATH;
			
			// Acabamos enviando la ruta por mensaje a la entidad para que alguien la capture
			if (message.Dispatch(*this))
				return LAStatus::SUCCESS;
			else
				return LAStatus::FAIL;
		}

	private:
		Vector3 _walkingSoulTarget; // TODO Nos debe venir por algún lado!
	};

}

#endif