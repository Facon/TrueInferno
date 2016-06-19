#include "LABurnEntity.h"

#include "Logic\Entity\ParticleType.h"

namespace AI {
	RTTI_IMPL(CLABurnEntity, CLatentAction);
	
	CLatentAction::LAStatus CLABurnEntity::OnStart() {
		ParticleMessage m(Logic::ParticleType::SOUL_DESTRUCTION, 0);
		bool result = m.Dispatch(*_entity);
		assert(result && "Can't start soul destruction particles");
		
		return LAStatus::RUNNING;
	}

}