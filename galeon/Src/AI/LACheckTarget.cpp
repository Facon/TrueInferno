#include "LACheckTarget.h"

#include "Logic\Server.h"

namespace AI {
	RTTI_IMPL(CLACheckTarget, CLatentAction);
	
	CLatentAction::LAStatus CLACheckTarget::OnStart() {
		CMap* map = Logic::CServer::getSingletonPtr()->getMap();
		CEntity* entity = map->getEntityByID(_smData.getTask()->getTarget());

		// Chequeamos que exista el objetivo (puede que haya sido destruído). Si no, fallamos
		if (entity == nullptr) {
			return LAStatus::FAIL;
		}

		else {
			return LAStatus::SUCCESS;
		}

	}
}