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
			return LAStatus::READY;
		}

		virtual LAStatus OnRun() {
			// Si no ha llegado petición nos suspendemos
			if (!_pathRequestReceived)
				return LAStatus::SUSPENDED;
			_pathRequestReceived = false;

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
		Vector3 _walkingSoulTarget;
		bool _pathRequestReceived;
	};

}

#endif