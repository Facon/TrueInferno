#include "LABurnEntity.h"

#include "Logic\Entity\ParticleType.h"

namespace AI {
	RTTI_IMPL(CLABurnEntity, CLatentAction);
	
	CLatentAction::LAStatus CLABurnEntity::OnStart() {
		ParticleMessage m(Logic::ParticleType::SOUL_DESTRUCTION, true);
		bool result = m.Dispatch(*_entity);
		
		if (result)
			return LAStatus::RUNNING;
		else
			return LAStatus::READY;
	}

}