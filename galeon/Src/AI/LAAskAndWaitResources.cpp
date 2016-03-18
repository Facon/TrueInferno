#include "LAAskAndWaitResources.h"
#include "Logic\Maps\EntityFactory.h"

namespace AI {
	CLatentAction::LAStatus CLAAskAndWaitResources::OnStart() {
		// Reiniciamos el tiempo pasado
		_elapsedTime = 0;

		// Calculamos los recursos necesarios para completar hasta el máximo
		int needed = _smData.getMaxResources() - _smData.getAvailableResources();

		// Si la cuenta sale negativa es que ya estamos a tope
		if (needed < 0)
			needed = 0;

		// Pedimos los recursos calculados
		LogisticsMessage m(LogisticsAction::NEED_RESOURCES, _resourceFrom, needed);
		
		// Intentamos transmitir la petición
		if (m.Dispatch(*_entity))
			// Y empezamos
			return LAStatus::RUNNING;
		else
			// O volveremos a intentarlo
			return LAStatus::READY;
	}

	CLatentAction::LAStatus CLAAskAndWaitResources::OnRun(unsigned int msecs) {
		// Finalizamos si ha transcurrido el tiempo límite
		_elapsedTime += msecs;
		if (_elapsedTime >= _period){
			return LAStatus::SUCCESS;
		}
		else{
			return LAStatus::RUNNING;
		}

	}
}