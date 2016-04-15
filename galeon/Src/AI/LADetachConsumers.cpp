#include "LADetachConsumers.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"
#include <map>

namespace AI {
	RTTI_IMPL(CLADetachConsumers, CLatentAction);
	
	CLatentAction::LAStatus CLADetachConsumers::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLADetachConsumers::OnRun(unsigned int msecs) {
		std::cout << "Detaching " << _smData.getConsumers().size() << " consumers..." << std::endl;

		// Preparamos el mensaje de desconexión
		PowerMessage powerMsg;
		powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), false, 0);

		// Mientras queden consumidores conectados
		auto it = _smData.getConsumers().begin();
		while (it != _smData.getConsumers().end()){
			TEntityID consumerId = it->first;

			// Localizamos la entidad
			CEntity* consumer = _entity->getMap()->getEntityByID(consumerId);

			// Si no es nula, le intentamos enviar el mensaje
			if (consumer != nullptr){
				// Reintentaremos el envío en el siguiente tick si no nos lo aceptan
				if (!powerMsg.Dispatch(*consumer))
					return LAStatus::RUNNING;
			}

			// Eliminamos al consumidor de nuestra estructura
			_smData.removeConsumer(consumerId);

			it = _smData.getConsumers().begin();
		}

		assert(_smData.getConsumers().size()==0 && "Some consumers were not detached");

		// Si había nuevo consumidor esperando respuesta, lo liberamos también
		if (_smData.getNewConsumer() != EntityID::UNASSIGNED){
			CEntity *consumer = _entity->getMap()->getEntityByID(_smData.getNewConsumer());
			if (consumer != nullptr){
				// Reintentamos el envío hasta que se acepte
				if (powerMsg.Dispatch(*consumer)){
					_smData.setNewConsumer(EntityID::UNASSIGNED);
				}
				else
					return LAStatus::RUNNING;
			}
		}

		/* Paramos el consumo si no lo estaba ya de por sí 
		* (es probable que hayamos entrado en este estado porque el consumidor se quedó sin recursos y nos lo solicitó) */
		ConsumptionMessage consumptionMsg;
		consumptionMsg.assembleConsumptionStop(ResourceType::COKE);

		if (!consumptionMsg.Dispatch(*_entity))
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}

}