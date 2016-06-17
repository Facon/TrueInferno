#ifndef LA_DESTROY_ENTITY_H_
#define LA_DESTROY_ENTITY_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLADestroyEntity : public CLatentAction {
		RTTI_DECL;

	public:
		CLADestroyEntity(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLADestroyEntity() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;
	};
}

#endif