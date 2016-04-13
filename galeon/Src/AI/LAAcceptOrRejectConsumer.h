#ifndef LA_ACCEPT_OR_REJECT_CONSUMER_H_
#define LA_ACCEPT_OR_REJECT_CONSUMER_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLAAcceptOrRejectConsumer : public CLatentAction {
		RTTI_DECL;

	public:
		CLAAcceptOrRejectConsumer(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData), _consumerNotified(false) {}

		virtual ~CLAAcceptOrRejectConsumer() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerGeneratorData& _smData;

		// Flag a true cuando el consumidor ya ha sido notificado
		bool _consumerNotified;

		// Flag a true cuando hemos notificado el cambio de consumo
		bool _consumptionChangeNotified;

		// Flag a true cuando estamos añadiendo el primer consumidor
		bool _firstConsumer;
		
		// Flag a true cuando estamos eliminando el útlimo consumidor
		bool _lastConsumer;

		// Flag a true cuando ya hemos notificado el primer consumidor
		bool _firstConsumerNotified;

		// Flag a true cuando ya hemos notificado el último consumidor
		bool _lastConsumerNotified;
	};
}

#endif // LA_ACCEPT_OR_REJECT_CONSUMER_H_