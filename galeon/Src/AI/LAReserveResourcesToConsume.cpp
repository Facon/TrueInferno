#include "LAReserveResourcesToConsume.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	RTTI_IMPL(CLAReserveResourcesToConsume, CLatentAction);
	
	bool CLAReserveResourcesToConsume::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de petici�n o reservas de un recurso distinto al esperado
		if (msg._type != MessageType::RESOURCES_RESERVED || msg._resourceType != _consumedResource)
			return false;

		// No se aceptan peticiones simult�neas
		if (_received)
			return false;

		_received = true;

		// Guardamos lo finalmente reservado
		// NOTA: En esta LA no se libera lo reservado. En otras LAs se conumir� (i.e. se reclamar� lo reservado) o se liberar�
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

		// Preparamos el mensaje de reserva de recursos que se enviar� a nuestra propia entidad
		ResourceMessage m;
		m.assembleResourcesReserve(_consumedResource, _smData.getConsumption(), _entity->getEntityID());

		// Reintentamos el mensaje hasta que se acepte
		if (m.Dispatch(*_entity))
			// Suspendemos la LA hasta que llegue el mensaje de confirmaci�n
			return LAStatus::SUSPENDED;
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAReserveResourcesToConsume::OnRun(unsigned int msecs) {
		if (!_received){
			assert(false && "No message received");
			return LAStatus::FAIL;
		}
		
		// Chequeamos si hemos podido reservar el consumo actual
		if (_smData.getReservedForConsume() == _smData.getConsumption()){
			return LAStatus::SUCCESS;
		}

		// O m�s
		else if (_smData.getReservedForConsume() > _smData.getConsumption()){
			// No deber�amos haber podido reservar m�s
			assert(false && "Reserved more resources than current consumption");
			return LAStatus::SUCCESS;
		}

		// O menos
		else {
			// Fallamos porque no se pudo reservar lo suficiente para el consumo
			return LAStatus::FAIL;
		}
	}

}