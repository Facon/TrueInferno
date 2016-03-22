#ifndef LA_RESERVE_PROVIDERS_H_
#define LA_RESERVE_PROVIDERS_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {
	class CLAReserveFromProviders : public CLatentAction {
	public:
		CLAReserveFromProviders(CEntity* entity, CSMResourceDemanderData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAReserveFromProviders() {}

	protected:
		LAStatus OnStart();

	private:
		CSMResourceDemanderData& _smData;
	};
}

#endif // LA_RESERVE_PROVIDERS_H_