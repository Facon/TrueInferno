#include "LAExecuteSoulTask.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	CLatentAction::LAStatus CLAExecuteSoulTask::OnRun() {
		// TODO Ejecutamos la tarea que... nos han tenido que pasar en alg�n lado :s
		std::cout << "Executing task" << std::endl;
		return LAStatus::SUCCESS;
	}
}