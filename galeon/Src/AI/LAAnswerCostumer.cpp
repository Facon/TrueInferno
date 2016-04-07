#include "LAAnswerCostumer.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	CLatentAction::LAStatus CLAAnswerCostumer::OnStart() {
		// Verificamos que existe el consumidor
		CEntity *consumer = _entity->getMap()->getEntityByID(_smData.getNewConsumer());
		if (consumer == nullptr)
			return LAStatus::FAIL;

		// Preparamos el mensaje con la conexión/desconexión
		PowerMessage m(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());
		
		// Reintentamos el envío hasta que se acepte
		if (m.Dispatch(*consumer))
			return LAStatus::SUCCESS;
		else
			return LAStatus::READY;
	}

}