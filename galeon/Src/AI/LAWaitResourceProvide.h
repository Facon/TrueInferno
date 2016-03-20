#ifndef LA_WAIT_RESOURCE_PROVIDE_H_
#define LA_WAIT_RESOURCE_PROVIDE_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceProviderData.h"

namespace Logic {
	class LogisticsMessage;
}

namespace AI {
	class CLAWaitResourceProvide : public CLatentAction {
	public:
		CLAWaitResourceProvide(CEntity* entity, CSMResourceProviderData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAWaitResourceProvide() {}

		virtual bool HandleMessage(const LogisticsMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceProviderData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_WAIT_RESOURCE_PROVIDE_H_