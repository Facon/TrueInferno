#include "LAFillReserves.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	RTTI_IMPL(CLAFillReserves, CLatentAction);

	CLatentAction::LAStatus CLAFillReserves::OnStart() {
		// Determinamos lo que necesitamos de coke
		int cokeNeeded = _smData.getTotalConsumptionUnits() + _smData.getNewConsumptionUnits() - _smData.getCurrentCoke();

		// Intentamos reservar lo que nos falta
		LogisticsMessage m;
		m.assembleDemandResources(ResourceType::COKE, cokeNeeded);

		// Repetimos el envío del mensaje hasta que nos lo acepten
		if (m.Dispatch(*_entity)){
			_smData.setNewConsumerAccepted(true); // TODO Necesitamos respuesta para saber si podemos aceptar!
			std::cout << "Filling=" << cokeNeeded << std::endl;
			return LAStatus::SUCCESS;
		}
		else
			return LAStatus::READY;
	}

}