#ifndef LA_ATTACH_TO_GENERATOR_H_
#define LA_ATTACH_TO_GENERATOR_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	class CLAAttachToGenerator : public CLatentAction {
	public:
		CLAAttachToGenerator(CEntity* entity, CSMPowerConsumerData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAAttachToGenerator() {}

		virtual bool HandleMessage(const PowerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerConsumerData& _smData;

		// Flag a true si ya hemos recibido respuesta en este tick
		bool _received;
	};
}

#endif // LA_ATTACH_TO_GENERATOR_H_