#include "LASendSoul.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLASendSoul::HandleMessage(const SoulSenderMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::SOUL_SENDER_REQUEST || _task != nullptr)
			return false;

		_task = msg._task;
		_numSouls = msg._numSouls;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLASendSoul::OnStart() {
		// Inicializamos
		if (_task){
			delete _task;
			_task = nullptr;
		}

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLASendSoul::OnRun() {
		// Verificación por seguridad
		if (_task == nullptr)
			return LAStatus::FAIL;

		return createAndSendSouls() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLASendSoul::createAndSendSouls(){
		bool ret = true;

		for (int i = 0; i < _numSouls; ++i){
			// Creamos alma
			CMap* map = Logic::CServer::getSingletonPtr()->getMap();
			CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

			if (!newSoul){
				assert(false && "Can´t create new soul");
				ret = false;
				continue;
			}

			// La ubicamos en nuestra propia posición
			PositionMessage m;
			m._type = MessageType::SET_POSITION;
			m._position = _entity->getPosition();
			m._position.y = SOUL_ON_TILE_HEIGHT;
			if (!m.Dispatch(*newSoul)){
				assert(false && "Can´t set soul on initial position");
				ret = false;
				continue;
			}

			// Le asignamos la tarea
			SoulMessage m2(_task->clone());
			if (!m2.Dispatch(*newSoul)){
				assert(false && "Can´assign task to soul");
				ret = false;
				continue;
			}
		}

		return ret;
	}
}