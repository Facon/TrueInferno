#include "LAWaitSoulTask.h"

namespace AI {
	RTTI_IMPL(CLAWaitSoulTask, CLatentAction);
	
	bool CLAWaitSoulTask::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una tarea a la vez
		if (msg._type != MessageType::SOUL_REQUEST || _smData.getTask() != nullptr)
			return false;

		// Guardamos la tarea
		_smData.setTask(msg._task);

		// Reactivamos la LA
		resume();
		
		return true;
	}

	CLatentAction::LAStatus CLAWaitSoulTask::OnStart() {
		// Inicializamos
		_smData.setTask(nullptr);

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitSoulTask::OnRun(unsigned int msecs) {
		// Fallamos si no hay tarea
		if (_smData.getTask() == nullptr)
			return LAStatus::FAIL;

		return LAStatus::SUCCESS;
	}

}