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
			/* Es seguro fallar porque lo �nico que podr�a faltar aqu� es, si acaso, deshacer los cambios en el consumidor.
			* Pero justo es el consumidor es el que ha desaparecido, por lo que nos podemos olvidar de �l */
			return LAStatus::FAIL;

		// Preparamos el mensaje con la conexi�n/desconexi�n
		PowerMessage powerMsg;
		powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());

		// Notificamos al consumidor
		if (!_consumerNotified){
			// Reintentamos el env�o hasta que se acepte
			if (powerMsg.Dispatch(*consumer))
				_consumerNotified = true;
			else
				return LAStatus::RUNNING;
		}

		// Y avisamos del cambio del consumo
		ConsumptionMessage consumptionMsg;

		// El cambio es positivo si es una conexi�n y negativo en caso contrario
		int consumptionChange = (_smData.getNewConsumerAccepted() ? 1 : -1) * _smData.getNewConsumption();

		consumptionMsg.assembleConsumptionChange(consumptionChange, ResourceType::COKE);

		if (consumptionMsg.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;

	}

}