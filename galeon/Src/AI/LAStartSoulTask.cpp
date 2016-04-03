#include "LAStartSoulTask.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	CLatentAction::LAStatus CLAStartSoulTask::OnStart() {
		// Intentamos empezar la tarea
		// TODO Cambiar bool por enumerado para abortar en caso de fallo. Actualmente los fallos no reintentables nos están devolviendo true!
		if (_smData.getTask()->start()){
			return LAStatus::SUCCESS;
		}

		// Si no se pudo arrancar esperamos al siguiente tick
		else
			return LAStatus::READY;
	}
}