#include "LAAttachToGenerator.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	bool CLAAttachToGenerator::HandleMessage(const PowerMessage& msg) {
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

	CLatentAction::LAStatus CLAAttachToGenerator::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAAttachToGenerator::OnRun(unsigned int msecs) {
		// ...
		return LAStatus::SUCCESS;
	}

}