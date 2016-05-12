#ifndef LA_FIND_GENERATOR_H_
#define LA_FIND_GENERATOR_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerConsumerData.h"
#include "BaseSubsystems\RTTI.h"

namespace AI {
	class CLAFindGenerator : public CLatentAction {
		RTTI_DECL;

	public:
		CLAFindGenerator(CEntity* entity, CSMPowerConsumerData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAFindGenerator() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerConsumerData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_FIND_GENERATOR_H_