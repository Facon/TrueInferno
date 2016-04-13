#ifndef LA_WAIT_SOUL_TASK_H_
#define LA_WAIT_SOUL_TASK_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAWaitSoulTask : public CLatentAction {
		RTTI_DECL;

	public:
		CLAWaitSoulTask(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAWaitSoulTask() {}

		virtual bool HandleMessage(const SoulMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;
	};
}

#endif // LA_WAIT_SOUL_TASK_H_