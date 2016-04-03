#include "LAFollowPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {

	CLatentAction::LAStatus CLAFollowPath::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAFollowPath::OnRun(unsigned int msecs) {
		// Fallamos si no hay ruta
		if (_smData.getPath() == nullptr)
			return LAStatus::FAIL;

		// Intentamos o reintentamos enviar la orden de seguir la ruta
		return sendPath() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAFollowPath::sendPath(){
		WalkSoulPathMessage m(_smData.getPath());
		m._type = MessageType::PERFORM_WALK_SOUL_PATH;

		return m.Dispatch(*_entity);
	}
}