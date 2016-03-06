#ifndef LA_EXECUTE_SOUL_TASK_
#define LA_EXECUTE_SOUL_TASK_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAExecuteSoulTask : public CLatentAction {
	public:
		CLAExecuteSoulTask(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAExecuteSoulTask() {}

	protected:
		LAStatus OnStart();
		LAStatus OnRun();

	private:
		CSMSoulData& _smData;

		bool CLAExecuteSoulTask::executeTask();
	};
}

#endif