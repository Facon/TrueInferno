#include "LAWaitGeneratorDetachment.h"

#include "Logic\Entity\Message.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	RTTI_IMPL(CLAWaitGeneratorDetachment, CLatentAction);
	
	bool CLAWaitGeneratorDetachment::HandleMessage(const PowerMessage& msg) {
		// Rechazamos lo que no sean mensajes de petición
		if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
			return false;

		// No se aceptan peticiones simultáneas
		if (_received)
			return false;
		
		// Registramos la recepción de mensaje en este click
		_received = true;

		// Registramos si se solicita desconexión
		_detach = !msg._attach;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnStart() {
		// Inicializamos
		_received = false;
		_detach = false;
		_generatorAssigned = EntityID::UNASSIGNED;

		// Suspendemos la LA hasta que llegue un mensaje de petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitGeneratorDetachment::OnRun(unsigned int msecs) {
		assert(_received && "No message received");

		// Si solicitaron desconexión
		if (_detach){
			// Eliminamos el generador al que estábamos conectados
			_smData.setPowerGenerator(EntityID::UNASSIGNED);
			_smData.setAttached(false);

			std::cout << "Detached:" << _entity->getEntityID() << std::endl;

			// La espera ha sido exitosa
			return LAStatus::SUCCESS;
		}

		// Si solicitaron conexión
		else{
			assert(false && "Can't change generator when waiting detachment");

			// Nos cambiamos de generador
			_smData.setPowerGenerator(_generatorAssigned);
			_smData.setAttached(true);
		
			// Seguimos esperando
			return LAStatus::RUNNING;
		}
	}

}