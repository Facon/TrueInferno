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
			// S�lo habr�a que deshacer la notificaci�n al consumidor pero, en realidad, es justo �l el que ya no existe!
			return LAStatus::FAIL;

		// Preparamos el mensaje con la conexi�n/desconexi�n
		PowerMessage m(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());

		// Notificamos PRIMERO al consumidor
		if (!_consumerNotified){
			// Reintentamos el env�o hasta que se acepte
			if (m.Dispatch(*consumer))
				_consumerNotified = true;
			else
				return LAStatus::RUNNING;
		}

		// Y DESPU�S al resto de componentes de nuestra entidad
		if (m.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;

	}

}