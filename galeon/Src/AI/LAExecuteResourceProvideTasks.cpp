#include "LAExecuteResourceProvideTasks.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"
#include "PerformTransportSoulTask.h"

namespace AI {
	RTTI_IMPL(CLAExecuteResourceProvideTasks, CLatentAction);
	
	CLatentAction::LAStatus CLAExecuteResourceProvideTasks::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExecuteResourceProvideTasks::OnRun(unsigned int msecs){
		// Preparamos el mensaje para que se envíe un alma hasta el objetivo portando el tipo y cantidad de los recursos dados
		SoulSenderMessage msg(new CPerformTransportSoulTask(
			_entity->getMap(), 
			_smData.getTarget(), 
			_smData.getResourceType(), 
			_smData.getResourceQuantity()), 
			1);

		// Solicitamos el envío de alma
		if (msg.Dispatch(*_entity))
			return LAStatus::SUCCESS;
		
		// Reintentamos en el siguiente tick si no hubo éxito
		else
			return LAStatus::RUNNING;
	}

}