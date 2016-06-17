#include "LAFollowPath.h"

#include "Logic\Entity\ParticleType.h"

namespace AI {
	RTTI_IMPL(CLAFollowPath, CLatentAction);

	CLatentAction::LAStatus CLAFollowPath::OnStart() {
		/* TODO Desactivadas partículas de momento
		// Al iniciar intentamos poner las partículas para el movimiento del alma
		ParticleMessage m(ParticleType::SOUL_MOVEMENT, true);
		if (m.Dispatch(*_entity))
			return LAStatus::RUNNING;
		else
			return LAStatus::READY;
		*/
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