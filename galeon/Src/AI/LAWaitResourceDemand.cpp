#include "LAWaitResourceDemand.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {
	RTTI_IMPL(CLAWaitResourceDemand, CLatentAction);
	
	bool CLAWaitResourceDemand::HandleMessage(const LogisticsMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::LOGISTICS_DEMAND_RESOURCES)
			return false;

		// No se aceptan peticiones simultáneas
		if (_requestReceived)
			return false;

		_requestReceived = true;

		// Guardamos datos en la memoria compartida de la SM
		_smData.setResourceType(msg._resourceType);
		_smData.setResourceQuantity(msg._resourceQuantity);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitResourceDemand::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitResourceDemand::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}