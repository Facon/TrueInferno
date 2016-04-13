#ifndef LA_WAIT_RESOURCE_DEMAND_H_
#define LA_WAIT_RESOURCE_DEMAND_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceDemanderData.h"

namespace Logic {
	class LogisticsMessage;
}

namespace AI {
	class CLAWaitResourceDemand : public CLatentAction {
		RTTI_DECL;

	public:
		CLAWaitResourceDemand(CEntity* entity, CSMResourceDemanderData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAWaitResourceDemand() {}

		virtual bool HandleMessage(const LogisticsMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceDemanderData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_WAIT_RESOURCE_DEMAND_H_