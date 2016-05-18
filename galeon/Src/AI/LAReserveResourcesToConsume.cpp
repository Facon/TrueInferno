#include "LAReserveResourcesToConsume.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceConsumerData.h"
#include <math.h>

namespace AI {
	RTTI_IMPL(CLAReserveResourcesToConsume, CLatentAction);
	
	bool CLAReserveResourcesToConsume::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición o reservas de un recurso distinto al esperado
		if (msg._type != MessageType::RESOURCES_RESERVED || msg._resourceType != _consumedResource)
			return false;

		// 	Rechazamos también si es una cantidad SUPERIOR a la esperada porque eso significa que la reserva no era para nosotros!
		if (msg._quantity > _resourcesAsked)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;

		_received = true;

		// Guardamos lo finalmente reservado
		// NOTA: En esta LA no se libera lo reservado. En otras LAs se debe consumir (i.e. reclamar lo reservado) o liberar la reserva
		_smData.setReservedForConsume(msg._quantity);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAReserveResourcesToConsume::OnStart() {
		// Inicializamos
		_received = false;
		_smData.setReservedForConsume(0);
		_resourcesAsked = 0;

		// Si no hay consumo ni de una unidad no hay nada que hacer
		if (ceil(_smData.getConsumption()) < 1)
			return LAStatus::SUCCESS;

		// Preparamos el mensaje de reserva de recursos que se enviará a nuestra propia entidad
		ResourceMessage m;
		_resourcesAsked = (int)ceil(_smData.getConsumption());
		m.assembleResourcesReserve(_consumedResource, _resourcesAsked, _entity->getEntityID());

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
		
		// Chequeamos si hemos podido reservar el consumo actual
		if (_smData.getReservedForConsume() == (int) ceil(_smData.getConsumption())){
			return LAStatus::SUCCESS;
		}

		// O más
		else if (_smData.getReservedForConsume() > _resourcesAsked){
			// No deberíamos haber podido reservar más
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