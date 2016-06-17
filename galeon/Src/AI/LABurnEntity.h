#ifndef LA_BURN_ENTITY_H_
#define LA_BURN_ENTITY_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLABurnEntity : public CLatentAction {
		RTTI_DECL;

	public:
		CLABurnEntity(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLABurnEntity() {}

	protected:
		LAStatus OnStart();

	private:
		CSMSoulData& _smData;
	};
}

#endif