#include "LAWaitLogisticsRequest.h"

#include "Logic\Entity\Message.h"
#include "AI\SMLogisticsData.h"

namespace AI {
	bool CLAWaitLogisticsRequest::HandleMessage(const LogisticsMessage& msg) {
		// Rechazamos lo que no sean mensajes de petici�n
		if (msg._type != MessageType::LOGISTICS_REQUEST)
			return false;

		// No se aceptan peticiones simult�neas
		if (_requestReceived)
			return false;

		_requestReceived = true;

		// TODO Guardamos datos

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitLogisticsRequest::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitLogisticsRequest::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}