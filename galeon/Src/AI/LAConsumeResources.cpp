#include "LAConsumeResources.h"

#include <math.h>

#include "Logic\ResourcesManager.h"
#include "Logic\Entity\ParticleType.h"

namespace AI {
	RTTI_IMPL(CLAConsumeResources, CLatentAction);
	
	CLatentAction::LAStatus CLAConsumeResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAConsumeResources::OnRun(unsigned int msecs) {
		// Si no ha hab�a nada reservado no hay nada que hacer
		if(_smData.getReservedForConsume() == 0)
			return LAStatus::SUCCESS;

		assert(_smData.getReservedForConsume() == (int) ceil(_smData.getConsumption()) && "Consumption and reserved for consume must be equal");

		// Enviamos un mensaje para consumir lo reservado
		ResourceMessage m;
		m.assembleResourcesClaim(_resourceType, _smData.getReservedForConsume());
		if (!m.Dispatch(*_entity))
			return LAStatus::RUNNING;

		//std::cout << "Consumed=" << _smData.getReservedForConsume() << std::endl;
		_smData.setReservedForConsume(0);

		// Activamos part�culas de acci�n de edificio
		ParticleMessage pm(ParticleType::BUILDING_ACTION, _consumptionParticlesDuration);
		bool result = pm.Dispatch(*_entity);
		assert(true && "Can't set building action particles");

		return LAStatus::SUCCESS; 
	}
}