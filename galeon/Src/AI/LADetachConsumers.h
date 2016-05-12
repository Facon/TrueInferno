#ifndef LA_DETACH_CONSUMERS_H_
#define LA_DETACH_CONSUMERS_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLADetachConsumers : public CLatentAction {
		RTTI_DECL;

	public:
		CLADetachConsumers(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLADetachConsumers() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerGeneratorData& _smData;
	};
}

#endif // LA_DETACH_CONSUMERS_H_