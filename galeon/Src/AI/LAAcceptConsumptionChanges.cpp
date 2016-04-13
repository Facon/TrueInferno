#include "LAAcceptConsumptionChanges.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	RTTI_IMPL(CLAAcceptConsumptionChanges, CLatentAction);
	
	bool CLAAcceptConsumptionChanges::HandleMessage(const ConsumptionMessage& msg) {
		// Rechazamos lo que no sean mensajes de cambio de consumo sobre el recurso que consumimos
		if (msg._type != MessageType::CONSUMPTION_CHANGE || msg._resourceType != _resourceType)
			return false;

		// Guardamos el cambio de consumo en la memoria compartida de la SM
		_smData.modifyConsumption(msg._consumptionChange);

		return true;
	}

	CLatentAction::LAStatus CLAAcceptConsumptionChanges::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAAcceptConsumptionChanges::OnRun(unsigned int msecs) {
		// TODO Se podr�a esperar mensajes durante un tiempo, pero entonces habr�a que asegurar que el consumo se produjera peri�dicamente
		return LAStatus::SUCCESS;
	}

}