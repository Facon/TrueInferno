#ifndef LA_WAIT_GENERATOR_DETACHMENT_H_
#define LA_WAIT_GENERATOR_DETACHMENT_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	class CLAWaitGeneratorDetachment : public CLatentAction {
		RTTI_DECL;

	public:
		CLAWaitGeneratorDetachment(CEntity* entity, CSMPowerConsumerData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAWaitGeneratorDetachment() {}

		virtual bool HandleMessage(const PowerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerConsumerData& _smData;

		// Flag a true si ya hemos recibido respuesta en este tick
		bool _received;

		// Flag a true si nos han indicado desconexión y false conexión
		bool _detach;
	};
}

#endif // LA_WAIT_GENERATOR_DETACHMENT_H_