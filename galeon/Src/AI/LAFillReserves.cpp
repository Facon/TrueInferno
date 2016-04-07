#include "LAFillReserves.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {

	CLatentAction::LAStatus CLAFillReserves::OnStart() {
		// Determinamos lo que necesitamos de coke
		int cokeNeeded = _smData.getTotalConsumption() + _smData.getNewConsumption() - _smData.getCurrentCoke();

		// Intentamos reservar lo que nos falta
		LogisticsMessage m;
		m.assembleDemandResources(ResourceType::COKE, cokeNeeded);

		if (m.Dispatch(*_entity)){
			_smData.setNewConsumerAccepted(true); // TODO Necesitamos respuesta para saber si podemos aceptar!
			return LAStatus::SUCCESS;
		}
		else
			return LAStatus::READY;
	}

}