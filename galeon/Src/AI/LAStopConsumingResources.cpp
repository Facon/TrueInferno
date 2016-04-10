#include "LAStopConsumingResources.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	CLatentAction::LAStatus CLAStopConsumingResources::OnStart() {
		// Si había recursos que liberar
		if (_smData.getReservedForConsume() > 0){
			// Liberamos lo que quedaba reservado
			ResourceMessage m;
			m.assembleResourcesFree(_resourceType, _smData.getReservedForConsume());

			// Reintentamos el envío hasta que se acepte
			if (m.Dispatch(*_entity)){
				// Limpiamos la reserva
				_smData.setReservedForConsume(0);
			}

			else
				return LAStatus::READY;
		}

		return LAStatus::RUNNING;
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