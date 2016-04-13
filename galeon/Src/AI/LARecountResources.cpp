#include "LARecountResources.h"

#include "Logic\Entity\Message.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	RTTI_IMPL(CLARecountResources, CLatentAction);
	
	bool CLARecountResources::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de información de recursos
		if (msg._type != MessageType::RESOURCES_INFO)
			return false;

		// Rechazamos si no es el tipo de recurso que queríamos saber
		if (msg._resourceType != _resourceFrom)
			return false;

		// En principio parece viable aceptar peticiones simultáneas. Si da problemas descomentar lo siguiente para evitarlas
		/*if (_requestReceived)
			return false;*/

		_requestReceived = true;

		// Guardamos la info en la memoria compartida
		_smData.setMaxResources(msg._max);
		_smData.setStoredResources(msg._stored);
		_smData.setAvailableResources(msg._available);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLARecountResources::OnStart() {
		// Inicializamos
		_requestReceived = false;

		// Solicitamos información de recursos a nosotros mismos
		ResourceMessage m;
		m.assembleResourcesAsk(_resourceFrom, _entity->getEntityID());
		
		// Si tenemos respuesta
		if (m.Dispatch(*_entity))
			// Suspenderemos la LA hasta que llegue un mensaje con los datos solicitados
			return LAStatus::SUSPENDED;

		// Si no, seguimos en OnStart
		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLARecountResources::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}