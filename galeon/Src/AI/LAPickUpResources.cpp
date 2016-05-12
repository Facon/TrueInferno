#include "LAPickUpResources.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	RTTI_IMPL(CLAPickUpResources, CLatentAction);
	
	bool CLAPickUpResources::HandleMessage(const ResourceMessage& msg) {
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

	CLatentAction::LAStatus CLAPickUpResources::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Recogemos los recursos que fueron reservados previamente
		ResourceMessage m;
		m.assembleResourcesClaim(_smData.getResourceType(), (int)_smData.getResourceQuantity());
		
		// Si lo aceptan es que se complet� la recolecci�n
		// TODO Ser�a conveniente enviar la petici�n de pago exigiendo confirmaci�n. En este punto pasar�amos a esperar dicho mensaje
		if (m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		// En otro caso reintentamos al siguiente tick
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAPickUpResources::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}