#include "LAGetTaskAndTarget.h"

namespace AI {
	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::HELLQUARTERS_REQUEST || _task != nullptr)
			return false;

		switch (msg._action){
		case SEND_SOUL_BURN:{
			CPlaceable *target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Furnace);
			//std::unique_ptr<CBurnTask> _task(new CBurnTask(target));
			_task = new CBurnTask(target);
			_numSouls = msg._numSouls;

			break;
		}

		case SEND_SOUL_WORK:{
			CPlaceable *target = CBuildingManager::getSingletonPtr()->getRandomBuilding();
			//std::unique_ptr<CWorkTask> _task(new CWorkTask(target));
			_task = new CWorkTask(target);
			_numSouls = msg._numSouls;

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

	CLatentAction::LAStatus CLAGetTaskAndTarget::OnRun() {
		// Verificación por seguridad
		if (_task == nullptr)
			return LAStatus::FAIL;

		return sendSoul() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAGetTaskAndTarget::sendSoul(){
		// Enviamos una copia de la tarea porque en este objeto la vamos a borrar
		SoulSenderMessage m(_task->clone(), _numSouls);
		return m.Dispatch(*_entity);
	}
}