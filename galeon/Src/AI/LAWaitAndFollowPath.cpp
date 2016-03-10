#include "LAWaitAndFollowPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAWaitAndFollowPath::HandleMessage(const WalkSoulPathMessage& msg) {
		// Rechazamos lo que no sean mensajes con la ruta
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH)
			return false;

		//No aceptamos más de una ruta simultáneamente
		if (_smData.getPath() != nullptr)
			return false;

		// Guardamos la ruta
		_smData.setPath(msg._path);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitAndFollowPath::OnStart() {
		// Inicializamos
		_smData.setPath(nullptr);

		// Suspendemos la LA hasta que llegue el mensaje con la ruta
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitAndFollowPath::OnRun(unsigned int msecs) {
		// Fallamos si no hay ruta
		if (_smData.getPath() == nullptr)
			return LAStatus::FAIL;

		// Intentamos o reintentamos enviar la orden de seguir la ruta
		return sendPath() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAWaitAndFollowPath::sendPath(){
		WalkSoulPathMessage m(_smData.getPath());
		m._type = MessageType::PERFORM_WALK_SOUL_PATH;

		return m.Dispatch(*_entity);
	}
}