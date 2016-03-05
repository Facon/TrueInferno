#include "LAWaitTaskAndRequestPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAWaitTaskAndRequestPath::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una tarea a la vez
		if (msg._type != MessageType::SOUL_REQUEST || _smData.getTask() != nullptr)
			return false;

		_smData.setTask(msg._task);

		// Reactivamos la LA
		resume();
		
		return true;
	}

	CLatentAction::LAStatus CLAWaitTaskAndRequestPath::OnStart() {
		// Inicializamos
		_smData.setPath(nullptr);

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitTaskAndRequestPath::OnRun() {
		// Fallamos si no hay tarea
		if (_smData.getTask() == nullptr)
			return LAStatus::FAIL;

		// Intentamos o reintentamos obtener ruta
		return requestPath() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAWaitTaskAndRequestPath::requestPath(){
		WalkSoulPathMessage m(_smData.getTask()->getTarget());
		return m.Dispatch(*_entity);
	}
}