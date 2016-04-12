#include "LAReserveResourcesToConsume.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	bool CLAReserveResourcesToConsume::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición o reservas de un recurso distinto al esperado
		if (msg._type != MessageType::RESOURCES_RESERVED || msg._resourceType != _consumedResource)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;

		_received = true;

		// Guardamos lo finalmente reservado
		_smData.setReservedForConsume(msg._quantity);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAReserveResourcesToConsume::OnStart() {
		// Inicializamos
		_received = false;
		_smData.setReservedForConsume(0);

		// Si no hay consumo, no hay nada que hacer
		if (_smData.getConsumption() == 0)
			return LAStatus::SUCCESS;

		// Preparamos el mensaje de reserva de recursos que se enviará a nuestra propia entidad
		ResourceMessage m;
		m.assembleResourcesReserve(_consumedResource, _smData.getConsumption(), _entity->getEntityID());

		// Reintentamos el mensaje hasta que se acepte
		if (m.Dispatch(*_entity))
			// Suspendemos la LA hasta que llegue el mensaje de confirmación
			return LAStatus::SUSPENDED;
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAReserveResourcesToConsume::OnRun(unsigned int msecs) {
		if (!_received){
			assert(false && "No message received");
			return LAStatus::FAIL;
		}
		
		// Chequeamos lo recibido vs el consumo actual
		if (_smData.getReservedForConsume() >= _smData.getConsumption()){
			return LAStatus::SUCCESS;
		}
		else{
			return LAStatus::FAIL;
		}
	}

}