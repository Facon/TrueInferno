#include "LAGetTaskAndTarget.h"

#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Maps\Managers\WorkManager.h"

namespace AI {
	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::HELLQUARTERS_REQUEST || _task != nullptr)
			return false;

		switch (msg._action){
		case SEND_SOUL_BURN:{
			// Buscamos horno. Si no hay, no aceptamos el mensaje
			TEntityID target = CWorkManager::getSingletonPtr()->findFurnace();
			// Si se devolvió el ID especial UNASSIGNED es que no había ningún edificio para trabajar
			if (target == EntityID::UNASSIGNED)
				return false;

			//std::unique_ptr<CBurnTask> _task(new CBurnTask(target));
			_task = new CBurnTask(_entity->getMap(), target);

			break;
		}

		case SEND_SOUL_WORK:{
			// Buscamos un edificio donde trabajar
			TEntityID target = CWorkManager::getSingletonPtr()->findBuildingToWork();

			// Si se devolvió el ID especial UNASSIGNED es que no había ningún edificio para trabajar
			if (target == EntityID::UNASSIGNED)
				return false;

			//std::unique_ptr<CWorkTask> _task(new CWorkTask(target));
			_task = new CWorkTask(_entity->getMap(), target);

			break;
		}

		default:{
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
		if (_task){
			delete _task;
			_task = nullptr;
		}

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGetTaskAndTarget::OnRun(unsigned int msecs) {
		// Verificación por seguridad
		if (_task == nullptr)
			return LAStatus::FAIL;

		return sendSoul() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAGetTaskAndTarget::sendSoul(){
		// Enviamos una copia de la tarea porque en este objeto la vamos a borrar
		SoulSenderMessage m(_task->clone(), 1);
		return m.Dispatch(*_entity);
	}
}