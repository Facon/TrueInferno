#include "LAConsumeResources.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	CLatentAction::LAStatus CLAConsumeResources::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAConsumeResources::OnRun(unsigned int msecs) {
		// Si no ha había nada reservado no hay nada que hacer
		if(_smData.getReservedForConsume() == 0)
			return LAStatus::SUCCESS;

		// Consumimos lo reservado
		ResourceMessage m;
		m.assembleResourcesClaim(_resourceType, _smData.getReservedForConsume());
		if (!m.Dispatch(*_entity)){
			std::cout << "Consumed=" << _smData.getReservedForConsume() << std::endl;
			_smData.setReservedForConsume(0);
			return LAStatus::RUNNING;
		}

		return LAStatus::SUCCESS;
	}
}