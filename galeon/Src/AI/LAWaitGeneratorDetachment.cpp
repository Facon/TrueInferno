#include "LAWaitGeneratorDetachment.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAWaitGeneratorDetachment::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petici�n
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simult�neas
		if (_requestReceived)
			return false;

		_requestReceived = true;

		// Guardamos datos en la memoria compartida de la SM
		//_smData.setXXX(msg._xxx);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnRun(unsigned int msecs) {
		// ...
		return LAStatus::SUCCESS;
	}

}