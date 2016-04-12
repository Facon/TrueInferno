#ifndef LA_FINDING_PATH_
#define LA_FINDING_PATH_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"

namespace AI {
	class CLAFindingPath : public CLatentAction{
	public:
		CLAFindingPath(CEntity* entity) : CLatentAction(entity), _walkingSoulTarget(EntityID::UNASSIGNED), _pathRequestReceived(false), _pathCalculated(nullptr) {
			_pathRequestReceived = false;
		}

		virtual ~CLAFindingPath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg)
		{
			// Rechazamos mensajes que no sean de petición y rechazamos también si ya había llegado uno
			if (msg._type != MessageType::REQUEST_WALK_SOUL_PATH || _pathRequestReceived)
				return false;

			// Hemos recibido petición
			_pathRequestReceived = true;

			// Guardamos el objetivo
			_walkingSoulTarget = msg._target;

			// Todavía no hemos calculado la ruta
			_pathCalculated = nullptr;

			// Reactivamos la LatentAction
			resume();

			return true;
		}

	protected:
		virtual LAStatus OnStart() {
			_walkingSoulTarget = EntityID::UNASSIGNED;
			_pathRequestReceived = false;
			_pathCalculated = false;
			return LAStatus::SUSPENDED;
		}

		virtual LAStatus OnRun(unsigned int msecs) {
			// Si no ha llegado petición, fallamos
			if (!_pathRequestReceived)
				return LAStatus::FAIL;

			// Localizamos la entidad objetivo
			Logic::CEntity* targetEntity = _entity->getMap()->getEntityByID(_walkingSoulTarget);

			// Controlamos que siga existiendo
			if (targetEntity == nullptr){
				std::cout << "Can't find path to target because target's entity no longer exists" << std::endl;
				_pathCalculated = nullptr;
			}

			// Si existe y aún no está calculada la ruta
			else if (_pathCalculated == nullptr)
				// Calculamos ruta desde la posición actual de la entidad hasta el objetivo
				_pathCalculated = AI::CServer::getSingletonPtr()->getWalkingSoulAStarRoute(_entity->getPosition(), targetEntity);

			// Preparamos el mensaje de vuelta
			WalkSoulPathMessage message(_pathCalculated);
			message._type = MessageType::RETURN_WALK_SOUL_PATH;
			
			// Enviamos el mensaje a nuestra entidad para informar de la ruta calculada
			if (message.Dispatch(*_entity))
				return LAStatus::SUCCESS;
			else
				return LAStatus::RUNNING; // Reintentamos si falla
		}

	private:
		// Identificador de la entidad a la que queremos ir
		TEntityID _walkingSoulTarget;

		// Flag que indica si se ha recibido petición de búsqueda de ruta
		bool _pathRequestReceived;

		// Path calculado. Puede ser nulo si no hay ruta posible
		std::vector<Vector3>* _pathCalculated;
	};

}

#endif