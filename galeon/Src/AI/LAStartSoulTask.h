#ifndef LA_START_SOUL_TASK_H_
#define LA_START_SOUL_TASK_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAStartSoulTask : public CLatentAction {
		RTTI_DECL;

	public:
		CLAStartSoulTask(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAStartSoulTask() {}

	protected:
		LAStatus OnStart();

	private:
		CSMSoulData& _smData;
	};
}

#endif // LA_START_SOUL_TASK_H_