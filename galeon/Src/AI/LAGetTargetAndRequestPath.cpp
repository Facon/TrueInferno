#include "LAGetTargetAndRequestPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAGetTargetAndRequestPath::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::SOUL_SENDER_REQUEST || _target != nullptr)
			return false;

		_target = msg._task->getTarget();

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAGetTargetAndRequestPath::OnStart() {
		// Inicializamos
		_target = nullptr;

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGetTargetAndRequestPath::OnRun() {
		// Verificación por seguridad
		if (_target == nullptr)
			return LAStatus::FAIL;

		return requestPath() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAGetTargetAndRequestPath::requestPath(){
		WalkSoulPathMessage m(_target);
		return m.Dispatch(*_entity);
	}
}