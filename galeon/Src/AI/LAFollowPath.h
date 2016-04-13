#ifndef LA_FOLLOW_PATH_H_
#define LA_FOLLOW_PATH_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAFollowPath : public CLatentAction {
		RTTI_DECL;

	public:
		CLAFollowPath(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAFollowPath() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;

		bool CLAFollowPath::sendPath();
	};
}

#endif