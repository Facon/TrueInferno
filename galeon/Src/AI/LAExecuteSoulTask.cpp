#include "LAExecuteSoulTask.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAExecuteSoulTask::HandleMessage(const WalkSoulPathMessage& msg) {
		// Rechazamos lo que no sean mensajes de aviso de ruta finalizada
		if (msg._type != MessageType::WALK_SOUL_PATH_FINISHED)
			return false;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAExecuteSoulTask::OnStart() {
		// Suspendemos la LA hasta que llegue el mensaje de aviso de ruta finalizada
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAExecuteSoulTask::OnRun() {
		// Verificamos que hay tarea que ejecutar
		if (_smData.getTask() == nullptr)
			return LAStatus::FAIL;

		return executeTask() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

	bool CLAExecuteSoulTask::executeTask(){
		// TODO
		/*
		WalkSoulPathMessage m(_path);
		m._type = MessageType::PERFORM_WALK_SOUL_PATH;

		return m.Dispatch(*_entity);
		*/

		std::cout << "TODO executeTask" << std::endl;
		return false;
	}
}