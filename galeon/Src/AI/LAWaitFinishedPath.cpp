#include "LAWaitFinishedPath.h"

namespace AI {
	RTTI_IMPL(CLAWaitFinishedPath, CLatentAction);
	
	bool CLAWaitFinishedPath::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean mensajes con la ruta
		if (msg._type != MessageType::SOUL_PATH_FINISHED)
			return false;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitFinishedPath::OnStart() {
		// Suspendemos la LA hasta que llegue el mensaje
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitFinishedPath::OnRun() {
		return LAStatus::SUCCESS;
	}
}