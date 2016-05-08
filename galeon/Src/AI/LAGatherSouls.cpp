#include "LAGatherSouls.h"
#include "Logic\Maps\EntityFactory.h"

namespace AI {
	RTTI_IMPL(CLAGatherSouls, CLatentAction);
	
	bool CLAGatherSouls::HandleMessage(const SoulBurnMessage& msg)
	{
		if (msg._type != MessageType::FURNACE_BURN_SOULS)
			return false;

		// Si la cantidad de almas que llegan sobrepasa la capacidad, rechazamos el paquete entero (as�
		// evitamos a�adir mensajes adicionales para informar del n� de almas aceptado).
		//     CUIDAO!: Esto podr�a traer problemas si se dejan de mandar todos los mensajes con
		//              _numSouls == 1, como hasta ahora.
		if ((msg._numSouls + _smData.getNumSoulsToBurn()) > _maxSoulsPerCycle)
			return false;

		// A�adimos las nuevas almas a ser quemadas
		for (int i = 0; i < msg._numSouls; ++i)
			_smData.addSoulToBurn(msg._soulsCategory);

		// Activamos la LA
		resume();

		// Iniciamos el contador de tiempo pasado
		_elapsedTime = 0;

		return true;
	}

	CLatentAction::LAStatus CLAGatherSouls::OnStart()
	{
		// Reiniciamos las almas a quemarse
		_smData.cleanSoulsToBurn();

		// Reiniciamos el tiempo pasado
		_elapsedTime = 0;

		// Se empieza la LA suspendida. Se despertar� con el primer alma que llegue a quemarse
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGatherSouls::OnRun(unsigned int msecs)
	{
		// Finalizamos si ha transcurrido el tiempo l�mite
		_elapsedTime += msecs;
		if (_elapsedTime >= _burnPeriod){
			return LAStatus::SUCCESS;
		}
		else{
			return LAStatus::RUNNING;
		}

	}
}