#include "LAConsumeResources.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	CLatentAction::LAStatus CLAConsumeResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAConsumeResources::OnRun(unsigned int msecs) {
		// Si no hay nada que consumir, finalizamos
		if(_smData.getConsumption() == 0)
			return LAStatus::SUCCESS;

		// Notificamos el consumo de recursos
		ResourceMessage m;
		m.assembleResourcesChange(_resourceType, -_smData.getConsumption());
		if (!m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}
}