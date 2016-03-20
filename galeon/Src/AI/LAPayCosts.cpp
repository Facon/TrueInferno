#include "LAPayCosts.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	bool CLAPayCosts::HandleMessage(const ResourceMessage& msg) {
		/* TODO De momento la confirmación de pago de costes no se envía por mensaje
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::RESOURCES_CHANGED)
			return false;

		// No se aceptan peticiones simultáneas
		if (_requestReceived)
			return false;

		_requestReceived = true;

		// Reactivamos la LA
		resume();

		return true;
		*/
		return false;
	}

	CLatentAction::LAStatus CLAPayCosts::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Se envía la solicitud de pago de costes
		ResourceMessage m;
		m.assembleResourcesChange(_smData.getResourceType(), _smData.getResourceQuantity());
		
		// Si lo aceptan es que se completó el pago
		// TODO Sería conveniente enviar la petición de pago exigiendo confirmación. En este punto pasaríamos a esperar dicho mensaje
		if (m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		// En otro caso reintentamos al siguiente tick
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAPayCosts::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}