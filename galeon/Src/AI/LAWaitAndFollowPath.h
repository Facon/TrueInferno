#ifndef LA_WAIT_FOLLOW_PATH_
#define LA_WAIT_FOLLOW_PATH_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAWaitAndFollowPath : public CLatentAction {
	public:
		CLAWaitAndFollowPath(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAWaitAndFollowPath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;

		bool CLAWaitAndFollowPath::sendPath();
	};
}

#endif