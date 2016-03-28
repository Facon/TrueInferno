#include "LAWaitTaskAndRequestPath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAWaitTaskAndRequestPath::HandleMessage(const SoulMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos m�s de una tarea a la vez
		if (msg._type != MessageType::SOUL_REQUEST || _smData.getTask() != nullptr)
			return false;

		// Guardamos la tarea
		_smData.setTask(msg._task);

		// Reactivamos la LA
		resume();
		
		return true;
	}

	CLatentAction::LAStatus CLAWaitTaskAndRequestPath::OnStart() {
		// Inicializamos
		_smData.setTask(nullptr);

		_taskStarted = false;

		// Suspendemos la LA hasta que llegue el mensaje con la petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAWaitTaskAndRequestPath::OnRun(unsigned int msecs) {
		// Fallamos si no hay tarea
		if (_smData.getTask() == nullptr)
			return LAStatus::FAIL;

		// Si la tarea todav�a no se ha arrancado
		if (!_taskStarted){
			// La arrancamos
			if (_smData.getTask()->start())
				_taskStarted = true;
			// Si no se pudo arrancar esperamos al siguiente tick
			else
				return LAStatus::RUNNING;
		}

		// Intentamos o reintentamos obtener ruta
		return requestPath() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAWaitTaskAndRequestPath::requestPath(){
		WalkSoulPathMessage m(_smData.getTask()->getTarget());
		return m.Dispatch(*_entity);
	}
}