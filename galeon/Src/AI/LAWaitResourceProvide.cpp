#include "LAWaitResourceProvide.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	RTTI_IMPL(CLAWaitResourceProvide, CLatentAction);
	
	bool CLAWaitResourceProvide::HandleMessage(const LogisticsMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::LOGISTICS_PROVIDE_RESOURCES)
			return false;

		// No se aceptan peticiones simultáneas
		if (_requestReceived)
			return false;

		_requestReceived = true;

		// Guardamos datos en la memoria compartida de la SM
		_smData.setResourceType(msg._resourceType);
		_smData.setResourceQuantity(msg._resourceQuantity);
		_smData.setTarget(msg._target);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitResourceProvide::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitResourceProvide::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}