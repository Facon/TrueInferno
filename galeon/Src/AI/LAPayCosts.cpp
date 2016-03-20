#include "LAPayCosts.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	bool CLAPayCosts::HandleMessage(const ResourceMessage& msg) {
		/* TODO De momento la confirmaci�n de pago de costes no se env�a por mensaje
		// Rechazamos lo que no sean mensajes de petici�n
		if (msg._type != MessageType::RESOURCES_CHANGED)
			return false;

		// No se aceptan peticiones simult�neas
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

		// Se env�a la solicitud de pago de costes
		ResourceMessage m;
		m.assembleResourcesChange(_smData.getResourceType(), _smData.getResourceQuantity());
		
		// Si lo aceptan es que se complet� el pago
		// TODO Ser�a conveniente enviar la petici�n de pago exigiendo confirmaci�n. En este punto pasar�amos a esperar dicho mensaje
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