#include "LAGetTaskAndTarget.h"

namespace AI {
	bool CLAGetTaskAndTarget::HandleMessage(const HellQuartersMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::HELLQUARTERS_REQUEST || _task != nullptr)
			return false;

		switch (msg._action){
		case SEND_SOUL_BURN:{
			_target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Furnace);
			_task = new CBurnTask();

			break;
		}
		case SEND_SOUL_WORK:{
			_target = CBuildingManager::getSingletonPtr()->getRandomBuilding();
			_task = new CWorkTask();
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
		_target = nullptr;

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAGetTaskAndTarget::OnRun() {
		// Verificación por seguridad
		if (_target == nullptr || _task == nullptr)
			return LAStatus::FAIL;

		return sendSoul() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAGetTaskAndTarget::sendSoul(){
		// TODO Enviar mensaje al SoulSender

		/*SoulSenderMessage() m;

		CMap* map = CServer::getSingletonPtr()->getMap();
		CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

		if (!newSoul){
			std::cout << "Can´t create new soul" << std::endl;
			return false;
		}

		// La ubicamos en la posición inicial de la ruta
		PositionMessage m;
		m._type = MessageType::SET_POSITION;
		m._position = (*_pathReceived)[0];
		m._position.y += SOUL_ON_TILE_HEIGHT;
		if (!m.Dispatch(*newSoul)){
			std::cout << "Can´t set soul on initial position" << std::endl;
			return false;
		}

		// Le indicamos la ruta que tiene que recorrer
		WalkSoulPathMessage m2(_pathReceived);
		m2._type = MessageType::PERFORM_WALK_SOUL_PATH;
		if (!m2.Dispatch(*newSoul)){
			std::cout << "Can´t assign path to soul" << std::endl;
			return false;
		}*/

		return true;
	}
}