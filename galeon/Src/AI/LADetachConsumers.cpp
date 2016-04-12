#include "LADetachConsumers.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"
#include <map>

namespace AI {
	CLatentAction::LAStatus CLADetachConsumers::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLADetachConsumers::OnRun(unsigned int msecs) {
		// Preparamos el mensaje de desconexión
		PowerMessage m;
		m.assemblePowerAttachmentInfo(_entity->getEntityID(), false, 0);

		// Mientras queden consumidores conectados
		auto it = _smData.getConsumers().begin();
		while (it != _smData.getConsumers().end()){
			TEntityID consumerId = it->first;

			// Localizamos la entidad
			CEntity* consumer = _entity->getMap()->getEntityByID(consumerId);

			// Si no es nula, le intentamos enviar el mensaje
			if (consumer != nullptr){
				// Reintentaremos el envío en el siguiente tick si no nos lo aceptan
				if (!m.Dispatch(*consumer))
					return LAStatus::RUNNING;
			}

			// Eliminamos al consumidor de nuestra estructura
			_smData.removeConsumer(consumerId);

			it = _smData.getConsumers().begin();
		}

		return LAStatus::SUCCESS;
	}

}