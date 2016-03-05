#ifndef LA_WAIT_TASK_REQUEST_PATH_
#define LA_WAIT_TASK_REQUEST_PATH_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {

	class CLAWaitTaskAndRequestPath : public CLatentAction {
	public:
		CLAWaitTaskAndRequestPath(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAWaitTaskAndRequestPath() {}

		virtual bool HandleMessage(const SoulMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun();

	private:
		CSMSoulData _smData;

		bool requestPath();
	};

}

#endif