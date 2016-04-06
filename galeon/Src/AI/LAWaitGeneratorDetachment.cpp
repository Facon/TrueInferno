#include "LAWaitGeneratorDetachment.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAWaitGeneratorDetachment::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petici�n
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simult�neas
		if (_received)
			return false;
		
		// Registramos la recepci�n de mensaje en este click
		_received = true;

		// Registramos si se solicita desconexi�n
		_detach = !msg._attach;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnStart() {
		// Inicializamos
		_received = false;

		// Suspendemos la LA hasta que llegue un mensaje de petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnRun(unsigned int msecs) {
		assert(_received && "No message received");

		// Si solicitaron desconexi�n la espera ha sido exitosa
		if (_detach)
			return LAStatus::SUCCESS;

		// Si no, seguimos esperando
		else
			return LAStatus::RUNNING;
	}

}