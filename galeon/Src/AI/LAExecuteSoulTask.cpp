#include "LAExecuteSoulTask.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	RTTI_IMPL(CLAExecuteSoulTask, CLatentAction);
	
	CLatentAction::LAStatus CLAExecuteSoulTask::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExecuteSoulTask::OnRun(unsigned int msecs) {
		// Verificamos que hay tarea que ejecutar
		if (_smData.getTask() == nullptr)
			return LAStatus::FAIL;

		return _smData.getTask()->execute() ? LAStatus::SUCCESS : LAStatus::RUNNING;
	}

}