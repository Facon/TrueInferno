#include "LAGetTaskAndTarget.h"

namespace AI {
	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos m�s de una petici�n
		if (msg._type != MessageType::HELLQUARTERS_REQUEST || _task != nullptr)
			return false;

		switch (msg._action){
		case SEND_SOUL_BURN:{
			_task = new CBurnSoulTask();
			_target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Furnace);

			break;
		}
		case SEND_SOUL_WORK:{
			_task = new CWorkSoulTask();
			_target = CBuildingManager::getSingletonPtr()->getRandomBuilding();
			break;
		}
		default:{
			assert(false && "HellQuartersAction not implemented");
			return false;
		}
		}

		resume();

		return true;
	}

	LAStatus CLAGetTaskAndTarget::OnStart() {
		// Inicializamos
		if (_task){
			delete _task;
			_task = nullptr;
		}
		_target = nullptr;

		// Dormimos la LA hasta que llegue el mensaje con la petici�n
		return LAStatus::SUSPENDED;
	}

	LAStatus CLAGetTaskAndTarget::OnRun() {
		// Verificaci�n por seguridad
		if (_target == nullptr || _task == nullptr)
			return LAStatus::FAIL;

		return sendSoul() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAGetTaskAndTarget::sendSoul(){
		CMap* map = CServer::getSingletonPtr()->getMap();
		CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

		if (!newSoul){
			std::cout << "Can�t create new soul" << std::endl;
			return false;
		}

		// La ubicamos en la posici�n inicial de la ruta
		PositionMessage m;
		m._type = MessageType::SET_POSITION;
		m._position = (*_pathReceived)[0];
		m._position.y += SOUL_ON_TILE_HEIGHT;
		if (!m.Dispatch(*newSoul)){
			std::cout << "Can�t set soul on initial position" << std::endl;
			return false;
		}

		// Le indicamos la ruta que tiene que recorrer
		WalkSoulPathMessage m2(_pathReceived);
		m2._type = MessageType::PERFORM_WALK_SOUL_PATH;
		if (!m2.Dispatch(*newSoul)){
			std::cout << "Can�t assign path to soul" << std::endl;
			return false;
		}

		return true;
	}
}