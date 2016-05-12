#ifndef LA_GET_SOUL_TASK_TARGET_H_
#define LA_GET_SOUL_TASK_TARGET_H_

#include "AI\LatentAction.h"
#include "AI\SMSoulData.h"

namespace AI {
	class CLAGetSoulTaskTarget : public CLatentAction {
		RTTI_DECL;

	public:
		CLAGetSoulTaskTarget(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAGetSoulTaskTarget() {}

	protected:
		LAStatus OnStart();

	private:
		CSMSoulData& _smData;
	};
}

#endif // LA_GET_SOUL_TASK_TARGET_H_