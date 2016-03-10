#ifndef LA_WAIT_LOGISTICS_REQUEST_
#define LA_WAIT_LOGISTICS_REQUEST_

#include "AI\LatentAction.h"
#include "AI\SMLogisticsData.h"

namespace Logic {
	class LogisticsMessage;
}

namespace AI {
	class CLAWaitLogisticsRequest : public CLatentAction {
	public:
		CLAWaitLogisticsRequest(CEntity* entity, CSMLogisticsData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAWaitLogisticsRequest() {}

		virtual bool HandleMessage(const LogisticsMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMLogisticsData _smData;

		bool _requestReceived;
	};
}

#endif