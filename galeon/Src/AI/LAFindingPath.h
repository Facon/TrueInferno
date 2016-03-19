#ifndef LA_FINDING_PATH_
#define LA_FINDING_PATH_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"

namespace AI {
	class CLAFindingPath : public CLatentAction{
	public:
		CLAFindingPath(CEntity* entity) : CLatentAction(entity) {
			_pathRequestReceived = false;
		}

		virtual ~CLAFindingPath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg)
		{
			// Rechazamos mensajes que no sean de petición y rechazamos también si ya había llegado uno
			if (msg._type != Logic::TMessage::REQUEST_WALK_SOUL_PATH || _pathRequestReceived)
				return false;

			_pathRequestReceived = true;

			_walkingSoulTarget = msg._target;

			// Reactivamos la LatentAction
			resume();

			return true;
		}

	protected:
		virtual LAStatus OnStart() {
			_pathRequestReceived = false;
			return LAStatus::SUSPENDED;
		}

		virtual LAStatus OnRun(unsigned int msecs) {
			// Si no ha llegado petición, fallamos
			if (!_pathRequestReceived)
				return LAStatus::FAIL;

			// Calculamos ruta desde la posición actual de la entidad hasta el objetivo
			std::vector<Vector3>* path = AI::CServer::getSingletonPtr()->getWalkingSoulAStarRoute(_entity->getPosition(), _entity->getMap()->getEntityByID(_walkingSoulTarget));

			// Reintentamos si no se encontró ruta
			if (path == nullptr){
				// TODO Revisar por qué no se encuentra ruta A VECES, y da igual que se reintente
				//std::cout << "Path not found :(" << std::endl;
				return LAStatus::RUNNING;
			}

			WalkSoulPathMessage message(path);
			message._type = MessageType::RETURN_WALK_SOUL_PATH;
			
			// Acabamos enviando la ruta por mensaje a la entidad para que alguien la capture
			if (message.Dispatch(*_entity))
				return LAStatus::SUCCESS;
			else
				return LAStatus::RUNNING; // Reintentamos si falla
		}

	private:
		TEntityID _walkingSoulTarget;
		bool _pathRequestReceived;
	};

}

#endif