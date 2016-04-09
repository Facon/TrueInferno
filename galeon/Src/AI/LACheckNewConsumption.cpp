#include "LACheckNewConsumption.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	bool CLACheckNewConsumption::HandleMessage(const ResourceMessage& msg) {
		// Rechazamos lo que no sean mensajes de información de recursos de coke
		if (msg._type != MessageType::RESOURCES_INFO || msg._resourceType != ResourceType::COKE)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;

		_received = true;

		// Guardamos datos en la memoria compartida de la SM
		_smData.setCurrentCoke(msg._quantity);

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLACheckNewConsumption::OnStart() {
		// Inicializamos
		_received = false;
		_smData.setCurrentCoke(0);

		// Preparamos un mensaje para saber cuánto coke tenemos
		ResourceMessage m;
		m.assembleResourcesAsk(ResourceType::COKE, _entity->getEntityID());

		// Repetimos hasta que se acepte el mensaje
		if (m.Dispatch(*_entity))
			// Nos suspendemos hasta que llegue la respuesta
			return LAStatus::SUSPENDED;

		else
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLACheckNewConsumption::OnRun(unsigned int msecs) {
		int totalConsumption = _smData.getTotalConsumption();
		int newConsumption = _smData.getNewConsumption();
		int currentCoke = _smData.getCurrentCoke();

		// Si el nuevo consumo total es aceptable: éxito
		if (currentCoke >= (totalConsumption + newConsumption)){
			_smData.setNewConsumerAccepted(true);
			return LAStatus::SUCCESS;
		}
		
		// Si no, fallo
		else{
			_smData.setNewConsumerAccepted(false);
			return LAStatus::FAIL;
		}
	}

}