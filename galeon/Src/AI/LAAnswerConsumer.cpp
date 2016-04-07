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
			// Sólo habría que deshacer la notificación al consumidor pero, en realidad, es justo él el que ya no existe!
			return LAStatus::FAIL;

		// Preparamos el mensaje con la conexión/desconexión
		PowerMessage m(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());

		// Notificamos PRIMERO al consumidor
		if (!_consumerNotified){
			// Reintentamos el envío hasta que se acepte
			if (m.Dispatch(*consumer))
				_consumerNotified = true;
			else
				return LAStatus::RUNNING;
		}

		// Y DESPUÉS al resto de componentes de nuestra entidad
		if (m.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;

	}

}