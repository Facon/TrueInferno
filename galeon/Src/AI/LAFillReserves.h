#ifndef LA_FILL_RESERVES_H_
#define LA_FILL_RESERVES_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLAFillReserves : public CLatentAction {
	public:
		CLAFillReserves(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAFillReserves() {}

	protected:
		LAStatus OnStart();

	private:
		CSMPowerGeneratorData& _smData;
	};
}

#endif // LA_FILL_RESERVES_H_