#include "LAExtractResources.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	CLatentAction::LAStatus CLAExtractResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExtractResources::OnRun(unsigned int msecs) {
		// TODO Hay que hacerlo proporcional al n�mero de trabajadores
		unsigned int quantity = _maxExtractedQuantity;

		// Notificamos el incremento de recursos
		ResourceMessage m;
		m.assembleResourcesChange(_resourceType, quantity);
		if (!m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}
}