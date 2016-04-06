#include "LAFindGenerator.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"
#include "Logic\Maps\Managers\PowerManager.h"

namespace AI {

	CLatentAction::LAStatus CLAFindGenerator::OnStart() {
		// Inicializamos
		_smData.setPowerGenerator(EntityID::UNASSIGNED);
		_smData.setAttached(false);

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAFindGenerator::OnRun(unsigned int msecs) {
		// Buscamos generador
		TEntityID _powerGenerator = CPowerManager::getSingletonPtr()->findPowerGenerator(_entity);
		
		// Y lo guardamos en la SM
		_smData.setPowerGenerator(_powerGenerator);

		// Éxito si encontramos alguno
		if (_powerGenerator != EntityID::UNASSIGNED)
			return LAStatus::SUCCESS;

		else
			return LAStatus::FAIL;
	}

}