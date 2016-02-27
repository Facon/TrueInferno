#include "LAGetTaskAndTarget.h"

namespace AI {
	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::HELLQUARTERS_REQUEST || _task != nullptr)
			return false;

		switch (msg._action){
		case SEND_SOUL_BURN:{
			CPlaceable *target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Furnace);
			_task = new CBurnTask(target);
			_numSouls = msg._numSouls;

			break;
		}
		case SEND_SOUL_WORK:{
			CPlaceable *target = CBuildingManager::getSingletonPtr()->getRandomBuilding();
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
		SoulSenderMessage m(_task, _numSouls);
		return m.Dispatch(*_entity);
	}
}