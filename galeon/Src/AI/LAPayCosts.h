#ifndef LA_PAY_COSTS_H_
#define LA_PAY_COSTS_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	class CLAPayCosts : public CLatentAction {
	public:
		CLAPayCosts(CEntity* entity, CSMResourceProviderData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAPayCosts() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceProviderData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_PAY_COSTS_H_