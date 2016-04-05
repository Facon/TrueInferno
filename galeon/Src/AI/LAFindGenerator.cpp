#include "LAFindGenerator.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {

	CLatentAction::LAStatus CLAFindGenerator::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAFindGenerator::OnRun(unsigned int msecs) {
		// ...
		return LAStatus::SUCCESS;
	}

}