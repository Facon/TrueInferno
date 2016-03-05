#include "LADestroyEntity.h"
#include "Logic\Maps\EntityFactory.h"

namespace AI {
	CLatentAction::LAStatus CLADestroyEntity::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLADestroyEntity::OnRun() {
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

		return LAStatus::SUCCESS;
	}
}