#ifndef LA_WAIT_GENERATOR_DETACHMENT_H_
#define LA_WAIT_GENERATOR_DETACHMENT_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	class CLAWaitGeneratorDetachment : public CLatentAction {
	public:
		CLAWaitGeneratorDetachment(CEntity* entity, CSMPowerConsumerData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAWaitGeneratorDetachment() {}

		virtual bool HandleMessage(const PowerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerConsumerData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_WAIT_GENERATOR_DETACHMENT_H_