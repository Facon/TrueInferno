#include "LAExecuteResourceDemandTasks.h"

#include "Logic\Entity\Message.h"
#include "Logic\Maps\Map.h"
#include "AI\SMResourceDemanderData.h"
#include "RequestTransportSoulTask.h"

namespace AI {
	RTTI_IMPL(CLAExecuteResourceDemandTasks, CLatentAction);
	
	CLatentAction::LAStatus CLAExecuteResourceDemandTasks::OnStart() {
		// Inicialización
		_nextProvider = 0;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExecuteResourceDemandTasks::OnRun(unsigned int msecs){
		// Obtenemos los mensajes de proveedores con las cantidades exactas a reservar
		std::vector<ResourceMessage> providerMessages = _smData.getProviderMessages();

		// Fallamos si no había ningún proveedor
		if (providerMessages.empty())
			return LAStatus::FAIL;

		// Para cada proveedor
		while (_nextProvider < (int)providerMessages.size()){
			ResourceMessage providerMessage = providerMessages[_nextProvider];

			// Saltamos aquellos a los que no tenemos que enviar alma porque no pudimos reservar nada
			if (providerMessage._quantity <= 0){
				++_nextProvider;
				continue;
			}

			/** Enviamos la petición de transporte de recursos desde el proveedor hacia nosotros.
			El alma se creará en la ubicación actual, viajará hacia el proveedor y volverá a nosotros con los recursos */
			SoulSenderMessage msg(new CRequestTransportSoulTask(
				_entity->getMap(), 	providerMessage._caller, _entity->getEntityID(), 
				providerMessage._resourceType, providerMessage._quantity), 1);
			
			// Intentamos enviar, reintentando en el siguiente tick si no nos aceptan el mensaje ahora
			if (!msg.Dispatch(*_entity))
				return LAStatus::RUNNING;

			// Avanzamos a la siguiente iteración
			++_nextProvider;
		}

		return LAStatus::SUCCESS;
	}

}