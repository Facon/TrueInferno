#ifndef LA_RESERVE_PROVIDERS_H_
#define LA_RESERVE_PROVIDERS_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {
	class CLAReserveFromProviders : public CLatentAction {
	public:
		CLAReserveFromProviders(CEntity* entity, CSMResourceDemanderData& smData) : 
			CLatentAction(entity), _smData(smData), _remainingToReserve(0), _nextProviderToReserve(0) {}

		virtual ~CLAReserveFromProviders() {}

		bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceDemanderData& _smData;

		/** Cantidad restante de recursos a reservar */
		int _remainingToReserve;

		/** Índice del siguiente proveedor del que solicitar reserva */
		int _nextProviderToReserve;
	};
}

#endif // LA_RESERVE_PROVIDERS_H_