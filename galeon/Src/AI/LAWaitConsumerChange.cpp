#include "LAWaitConsumerChange.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	RTTI_IMPL(CLAWaitConsumerChange, CLatentAction);
	
	bool CLAWaitConsumerChange::HandleMessage(const PowerMessage& msg) {
		// Aceptamos desconexiones siempre
		if (msg._type == MessageType::POWER_ATTACHMENT_INFO && msg._attach == false){
			// Eliminamos al consumidor internamente
			_smData.removeConsumer(msg._caller);

			return true;
		}

		// Aceptamos peticiones de conexión
		else if (msg._type == MessageType::POWER_REQUEST_ATTACHMENT){
			// No se aceptan peticiones simultáneas
			if (_received)
				return false;

			_received = true;

			// Guardamos datos en la memoria compartida de la SM
			_smData.setNewConsumer(msg._caller);
			_smData.setNewConsumption(msg._consumption);

			// Reactivamos la LA para chequear el nuevo consumo
			resume();

			return true;
		}

		// Descartamos el resto
		else{
			assert(false && "No logic implemented for this PowerMessage case");
			return false;
		}
	}

	CLatentAction::LAStatus CLAWaitConsumerChange::OnStart() {
		// Inicializamos
		_received = false;
		_smData.setNewConsumer(EntityID::UNASSIGNED);
		_smData.setNewConsumption(0);
		_smData.setNewConsumerAccepted(false);

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitConsumerChange::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}