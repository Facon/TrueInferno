#ifndef LA_CHECK_NEW_CONSUMPTION_H_
#define LA_CHECK_NEW_CONSUMPTION_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLACheckNewConsumption : public CLatentAction {
		RTTI_DECL;

	public:
		CLACheckNewConsumption(CEntity* entity, CSMPowerGeneratorData& smData) : 
			CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLACheckNewConsumption() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerGeneratorData& _smData;

		bool _received;
	};
}

#endif // LA_CHECK_NEW_CONSUMPTION_H_