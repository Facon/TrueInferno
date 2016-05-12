#include "LADestroyEntity.h"
#include "Logic\Maps\EntityFactory.h"

namespace AI {
	RTTI_IMPL(CLADestroyEntity, CLatentAction);
	
	CLatentAction::LAStatus CLADestroyEntity::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLADestroyEntity::OnRun(unsigned int msecs) {
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

		return LAStatus::SUCCESS;
	}
}