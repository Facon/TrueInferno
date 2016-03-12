#include "LAFindProviders.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"
#include "Logic\BuildingManager.h"

namespace AI {
	bool CLAFindProviders::HandleMessage(const LogisticsMessage& msg) {
		// Rechazamos lo que no sean mensajes de información de recursos
		if (msg._action != LogisticsAction::I_HAVE_RESOURCES)
			return false;

		// Guardamos los mensajes recibidos
		_smData.addProviderMessage(msg);

		return true;
	}

	CLatentAction::LAStatus CLAFindProviders::OnStart() {
		// Inicializamos
		_smData.clearProviderMessages();
		_elapsedTime = 0;

		// Creamos el mensaje para preguntar disponibilidad de recursos
		LogisticsMessage msg(LogisticsAction::DO_YOU_HAVE_RESOURCES, _smData.getResourceType());

		// Preguntamos a todos los edificios con el mensaje creado
		if(Logic::CBuildingManager::getSingletonPtr()->HandleMessage(msg))
			return LAStatus::RUNNING;
		else
			// Fallamos si no respondió ninguno
			return LAStatus::FAIL;
	}

	CLatentAction::LAStatus CLAFindProviders::OnRun(unsigned int msecs) {
		if ((_elapsedTime += msecs) >= _maxWaitingTime)
			return LAStatus::SUCCESS;
		else
			return LAStatus::RUNNING;
	}

}