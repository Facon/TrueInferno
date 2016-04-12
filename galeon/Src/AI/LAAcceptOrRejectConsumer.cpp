#include "LAAcceptOrRejectConsumer.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	CLatentAction::LAStatus CLAAcceptOrRejectConsumer::OnStart() {
		// Inicializamos
		_consumerNotified = false;
		_consumptionChangeNotified = false;
		_firstConsumer = false;
		_lastConsumer = false;
		_firstConsumerNotified = false;
		_lastConsumerNotified = false;

		// Añadimos o eliminamos al consumidor en la lista de conexiones de consumidores del PowerGenerator
		if (_smData.getNewConsumerAccepted()){
			bool added = _smData.addConsumer(_smData.getNewConsumer(), _smData.getNewConsumption());

			// Si se añadió consumidor y la lista tiene un único consumidor es que es el primero
			if (added && _smData.getNumConsumers() == 1)
				_firstConsumer = true;
		}

		else{
			bool removed = _smData.removeConsumer(_smData.getNewConsumer());
			
			// Si se eliminó consumidor y la lista está vacía es que era el último
			if (removed && _smData.getNumConsumers() == 0)
				_lastConsumer = true;
		}

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAAcceptOrRejectConsumer::OnRun(unsigned int msecs) {
		/* Hacemos 3 cosas:
		* 1) Notificar a la entidad del consumidor de su conexión/desconexión
		* 2) Notificar a nuestra entidad del cambio en el consumo
		* 3) Notificar a nuestra entidad si era el primer consumidor o el último para empezar o parar el consumo (OJO: parar el consumo DESPUÉS de notificar el cambio)
		*/

		// Notificamos al consumidor de la conexión/desconexión si no lo habíamos hecho ya
		if (!_consumerNotified){
			// Verificamos que existe el consumidor
			CEntity *consumer = _entity->getMap()->getEntityByID(_smData.getNewConsumer());
			if (consumer != nullptr){
				// Preparamos el mensaje con la conexión/desconexión
				PowerMessage powerMsg;
				powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), _smData.getNewConsumerAccepted(), _smData.getNewConsumption());

				// Reintentamos el envío hasta que se acepte
				if (powerMsg.Dispatch(*consumer))
					_consumerNotified = true;

				else
					return LAStatus::RUNNING;
			}

			else{
				// Damos por notificado al consumidor porque ha desaparecido
				_consumerNotified = true;
			}
		}

		// Notificamos el cambio de consumo si no estaba hecho ya
		if (!_consumptionChangeNotified){
			// Avisamos del cambio del consumo
			ConsumptionMessage consumptionMsg;

			// El cambio es positivo si es una conexión y negativo en caso contrario
			int consumptionChange = (_smData.getNewConsumerAccepted() ? 1 : -1) * _smData.getNewConsumption();

			consumptionMsg.assembleConsumptionChange(consumptionChange, ResourceType::COKE);

			if (consumptionMsg.Dispatch(*_entity))
				_consumptionChangeNotified = true;

			else
				return LAStatus::RUNNING;
		}

		// Si era el primer consumidor y todavía no lo hemos notificado
		if (_firstConsumer && !_firstConsumerNotified){
			// Ordenamos que empiece el consumo
			ConsumptionMessage m;
			m.assembleConsumptionStart(ResourceType::COKE);

			// Reintentamos el envío hasta que se acepte
			if (m.Dispatch(*_entity))
				_firstConsumerNotified = true;

			else
				return LAStatus::RUNNING;
		}

		// Si era el último consumidor y todavía no lo hemos notificado
		else if (_lastConsumer && !_lastConsumerNotified){
			// Ordenamos que pare el consumo
			ConsumptionMessage m;
			m.assembleConsumptionStop(ResourceType::COKE);

			// Reintentamos el envío hasta que se acepte
			if (m.Dispatch(*_entity))
				_lastConsumerNotified = true;

			else
				return LAStatus::RUNNING;
		}

		return LAStatus::SUCCESS;
	}

}