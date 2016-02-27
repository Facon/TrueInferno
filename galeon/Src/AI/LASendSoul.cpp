#include "LASendSoul.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLASendSoul::HandleMessage(const SoulSenderMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos m�s de una petici�n simult�nea
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

		// Suspendemos la LA hasta que llegue el mensaje con la petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLASendSoul::OnRun() {
		// Verificaci�n por seguridad
		if (_task == nullptr)
			return LAStatus::FAIL;

		return createAndSendSoul() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLASendSoul::createAndSendSoul(){
		CMap* map = Logic::CServer::getSingletonPtr()->getMap();
		CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

		if (!newSoul){
			assert(false && "Can�t create new soul");
			return false;
		}

		// TODO Enviar posici�n inicial y tarea a la vez a las N almas

		// La ubicamos en nuestra propia posici�n
		/*PositionMessage m;
		m._type = MessageType::SET_POSITION;
		m._position = _entity->getPosition();
		m._position.y = SOUL_ON_TILE_HEIGHT;
		if (!m.Dispatch(*newSoul)){
			assert(false && "Can�t set soul on initial position");
			return false;
		}

		return true;*/

		// Le indicamos la ruta que tiene que recorrer
		/*WalkSoulPathMessage m2(_pathReceived);
		m2._type = MessageType::PERFORM_WALK_SOUL_PATH;
		if (!m2.Dispatch(*newSoul)){
			std::cout << "Can�t assign path to soul" << std::endl;
			return false;
		}*/

		return true;
	}
}