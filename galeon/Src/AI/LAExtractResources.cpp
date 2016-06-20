#include "LAExtractResources.h"

#include "Logic\ResourcesManager.h"
#include "Logic\Entity\ParticleType.h"

namespace AI {
	RTTI_IMPL(CLAExtractResources, CLatentAction);
	
	CLatentAction::LAStatus CLAExtractResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExtractResources::OnRun(unsigned int msecs) {
		// La cantidad de recursos extraídos es la máxima ajustada por el factor de velocidad
		int quantity = (int) std::truncf(_maxExtractedQuantity * _smData.getSpeedFactor());

		if (quantity == 0)
			return LAStatus::SUCCESS;

		assert((quantity >= 0) && "Extracted quantity must be >= 0");

		// Notificamos el incremento de recursos
		ResourceMessage rm;
		rm.assembleResourcesChange(_resourceType, quantity);
		if (!rm.Dispatch(*_entity))
			return LAStatus::RUNNING;

		// Activamos partículas de acción de edificio
		ParticleMessage pm(ParticleType::BUILDING_ACTION, _extractParticlesDuration);
		bool result = pm.Dispatch(*_entity);
		assert(true && "Can't set building action particles");

		return LAStatus::SUCCESS;
	}
}