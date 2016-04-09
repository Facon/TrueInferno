#include "LAAnswerConsumer.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	CLatentAction::LAStatus CLAAnswerConsumer::OnStart() {
		// Inicializamos
		_consumerNotified = false;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAAnswerConsumer::OnRun(unsigned int msecs) {
		// Verificamos que existe el consumidor
		CEntity *consumer = _entity->getMap()->getEntityByID(_smData.getNewConsumer());
		if (consumer == nullptr)
			/* Es seguro fallar porque lo único que podría faltar aquí es, si acaso, deshacer los cambios en el consumidor.
			* Pero justo es el consumidor es el que ha desaparecido, por lo que nos podemos olvidar de él */
			return LAStatus::FAIL;

		// Preparamos el mensaje con la conexión/desconexión
		PowerMessage powerMsg;
		powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());

		// Notificamos al consumidor
		if (!_consumerNotified){
			// Reintentamos el envío hasta que se acepte
			if (powerMsg.Dispatch(*consumer))
				_consumerNotified = true;
			else
				return LAStatus::RUNNING;
		}

		// Y avisamos del cambio del consumo
		ConsumptionMessage consumptionMsg;

		// El cambio es positivo si es una conexión y negativo en caso contrario
		int consumptionChange = (_smData.getNewConsumerAccepted() ? 1 : -1) * _smData.getNewConsumption();

		consumptionMsg.assembleConsumptionChange(consumptionChange, ResourceType::COKE);

		if (consumptionMsg.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;

	}

}