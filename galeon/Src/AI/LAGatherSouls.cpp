#include "LAGatherSouls.h"
#include "Logic\Maps\EntityFactory.h"

namespace AI {
	RTTI_IMPL(CLAGatherSouls, CLatentAction);
	
	bool CLAGatherSouls::HandleMessage(const NumberMessage& msg){
		if (msg._type != MessageType::FURNACE_BURN_SOULS)
			return false;

		// Si la cantidad de almas que llegan sobrepasa la capacidad, rechazamos el paquete entero (así evitamos añadir mensajes adicionales para informar del nº de almas aceptado)
		if ((msg._number + _smData.getNumSoulsToBurn()) > _maxSoulsPerCycle)
			return false;

		// Aumentamos el número de almas para ser quemadas
		_smData.increaseNumSoulsToBurn(msg._number);

		// Activamos la LA
		resume();

		// Iniciamo el contador de tiempo pasado
		_elapsedTime = 0;

		return true;
	}


	CLatentAction::LAStatus CLAGatherSouls::OnStart() {
		// Reiniciamos el número de almas a quemarse
		_smData.setNumSoulsToBurn(0);

		// Reiniciamos el tiempo pasado
		_elapsedTime = 0;

		// Se empieza la LA suspendida. Se despertará con la primera alma que llegue a quemarse
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGatherSouls::OnRun(unsigned int msecs) {
		// Finalizamos si ha transcurrido el tiempo límite
		_elapsedTime += msecs;
		if (_elapsedTime >= _burnPeriod){
			return LAStatus::SUCCESS;
		}
		else{
			return LAStatus::RUNNING;
		}

	}
}