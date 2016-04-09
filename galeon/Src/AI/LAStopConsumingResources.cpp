#include "LAStopConsumingResources.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	CLatentAction::LAStatus CLAStopConsumingResources::OnStart() {
		// Liberamos lo que quedara reservado
		ResourceMessage m;
		m.assembleResourcesFree(_resourceType, _smData.getReservedForConsume());

		// Reintentamos el envío hasta que se acepte
		if (m.Dispatch(*_entity)){
			_smData.setReservedForConsume(0);
			return LAStatus::RUNNING;
		}
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAStopConsumingResources::OnRun(unsigned int msecs) {
		// Notificamos que se ha dejado de consumir
		ConsumptionMessage m;
		m.assembleConsumptionStopped(_resourceType);

		// Reintentamos el envío hasta que se acepte
		if (m.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;
	}

}