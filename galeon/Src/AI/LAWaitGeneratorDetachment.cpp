#include "LAWaitGeneratorDetachment.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAWaitGeneratorDetachment::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;
		
		// Registramos la recepción de mensaje en este click
		_received = true;

		// Registramos si se solicita desconexión
		_detach = !msg._attach;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnStart() {
		// Inicializamos
		_received = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnRun(unsigned int msecs) {
		assert(_received && "No message received");

		// Si solicitaron desconexión la espera ha sido exitosa
		if (_detach)
			return LAStatus::SUCCESS;

		// Si no, seguimos esperando
		else
			return LAStatus::RUNNING;
	}

}