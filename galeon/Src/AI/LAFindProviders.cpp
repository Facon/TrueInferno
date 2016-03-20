#include "LAFindProviders.h"

#include "Logic\Entity\Message.h"
#include "Logic\BuildingManager.h"

namespace AI {
	bool CLAFindProviders::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de información de recursos
		if (msg._type != MessageType::RESOURCES_INFO)
			return false;

		// Guardamos la información si la entidad provee externamente el recurso y tiene algo que ofrecer
		if (msg._provides && msg._available > 0)
			_smData.addProviderMessage(msg);

		return true;
	}

	CLatentAction::LAStatus CLAFindProviders::OnStart() {
		// Inicializamos
		_smData.clearProviderMessages();
		_elapsedTime = 0;

		// Creamos el mensaje para preguntar disponibilidad de recursos. Indicamos que somos nosotros los que preguntamos la info
		ResourceMessage msg;
		msg.assembleResourcesAsk(_smData.getResourceType(), _entity->getEntityID());

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