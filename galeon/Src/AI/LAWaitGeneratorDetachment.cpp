#include "LAWaitGeneratorDetachment.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAWaitGeneratorDetachment::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simultáneas
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

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnRun(unsigned int msecs) {
		// ...
		return LAStatus::SUCCESS;
	}

}