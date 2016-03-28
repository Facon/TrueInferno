#ifndef LA_FIND_PROVIDERS_
#define LA_FIND_PROVIDERS_

#include "AI\LatentAction.h"
#include "AI\SMResourceDemanderData.h"
#include <vector>

namespace AI {
	class CLAFindProviders : public CLatentAction {
	public:
		CLAFindProviders(CEntity* entity, CSMResourceDemanderData& smData) : 
			CLatentAction(entity), 
			_smData(smData),
			_maxWaitingTime(1000) {}

		virtual ~CLAFindProviders() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceDemanderData& _smData;

		/** Tiempo (ms) que se espera a recibir respuestas de los proveedores */
		unsigned int _maxWaitingTime;

		/** Tiempo (ms) transcurrido desde que empezó la acción */
		unsigned int _elapsedTime;
	};
}

#endif