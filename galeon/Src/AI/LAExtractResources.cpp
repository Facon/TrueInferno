#include "LAExtractResources.h"

#include <math.h>

#include "Logic\ResourcesManager.h"

namespace AI {
	RTTI_IMPL(CLAExtractResources, CLatentAction);
	
	CLatentAction::LAStatus CLAExtractResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExtractResources::OnRun(unsigned int msecs) {
		// La cantidad de recursos extraídos es la máxima ajustada por el factor de velocidad
		int quantity = (int) std::truncf(_maxExtractedQuantity * _smData.getSpeedFactor());

		// Notificamos el incremento de recursos
		ResourceMessage m;
		m.assembleResourcesChange(_resourceType, quantity);
		if (!m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}
}