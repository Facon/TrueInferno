#ifndef LA_WAIT_CONSUMER_CHANGE_H_
#define LA_WAIT_CONSUMER_CHANGE_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLAWaitConsumerChange : public CLatentAction {
	public:
		CLAWaitConsumerChange(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAWaitConsumerChange() {}

		virtual bool HandleMessage(const PowerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerGeneratorData& _smData;

		bool _received;
	};
}

#endif // LA_WAIT_CONSUMER_CHANGE_H_