#include "LAGetTaskAndTarget.h"

#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Maps\Managers\WorkManager.h"

namespace AI {
	RTTI_IMPL(CLAGetTaskAndTarget, CLatentAction);

	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg)
	{
		// Rechazamos lo que no sean peticiones
		if (msg._type != MessageType::HELLQUARTERS_REQUEST)
			return false;

		switch (msg._action)
		{
			case HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN:
			{
				_pendingSoulsToBurn += msg._numSouls;
				break;
			}

			case HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK:
			{
				_pendingSoulsToWork += msg._numSouls;
				break;
			}

			default:
			{
				assert(false && "HellQuartersAction not implemented");
				return false;
			}
		}

		// Reactivamos la LA
		resume();
		return true;
	}

	CLatentAction::LAStatus CLAGetTaskAndTarget::OnStart() {
		// Inicializamos
		if (_workTask)
			deleteTask(_workTask);

		if (_burnTask)
			deleteTask(_burnTask);
		
		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGetTaskAndTarget::OnRun(unsigned int msecs)
	{
		if (_pendingSoulsToWork == 0 && _pendingSoulsToBurn == 0)
			// No hay almas por crear. Suspendemos hasta nueva petición
			return LAStatus::SUSPENDED;

		bool workTask = false;
		bool burnTask = false;

		// Creación de tareas
		if (_pendingSoulsToWork > 0)
			// Crear y enviar alma a trabajar
			workTask = createWorkTask();

		if (_pendingSoulsToBurn > 0)
			// Crear y enviar alma a quemarse
			burnTask = createBurnTask();

		// Envío de almas
		bool sendWork = false;
		bool sendBurn = false;

		if (workTask)
		{
			sendWork = sendSoul(true);

			if (sendWork)
				--_pendingSoulsToWork;
		}

		if (burnTask)
		{
			sendBurn = sendSoul(false);

			if (sendBurn)
				--_pendingSoulsToBurn;
		}

		// Devolvemos siempre RUNNING, independientemente de que hayan podido crearse almas en
		// este tick, luego se seguirá ejecutando hasta que no haya más almas por crear.
		return LAStatus::RUNNING;
	}

	bool CLAGetTaskAndTarget::createWorkTask()
	{
		// Buscamos un edificio donde trabajar
		TEntityID target = CWorkManager::getSingletonPtr()->findBuildingToWork();

		// Si se devolvió el ID especial UNASSIGNED es que no había ningún edificio para trabajar
		if (target == EntityID::UNASSIGNED)
			return false;

		//std::unique_ptr<CWorkTask> _task(new CWorkTask(target));
		_workTask = new CWorkTask(_entity->getMap(), target);
		return true;
	}

	bool CLAGetTaskAndTarget::createBurnTask()
	{
		// Buscamos horno. Si no hay, no aceptamos el mensaje
		TEntityID target = CWorkManager::getSingletonPtr()->findFurnace();

		// Si se devolvió el ID especial UNASSIGNED es que no había ningún edificio para trabajar
		if (target == EntityID::UNASSIGNED)
			return false;

		//std::unique_ptr<CBurnTask> _task(new CBurnTask(target));
		_burnTask = new CBurnTask(_entity->getMap(), target);
		return true;
	}

	bool CLAGetTaskAndTarget::sendSoul(bool workTask)
	{
		// Enviamos una copia de la tarea porque en este objeto la vamos a borrar
		CSoulTask *task = (workTask) ? _workTask : _burnTask;

		SoulSenderMessage m(task->clone(), 1);
		return m.Dispatch(*_entity);
	}
}