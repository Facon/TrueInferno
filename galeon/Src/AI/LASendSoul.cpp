#include "LASendSoul.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	RTTI_IMPL(CLASendSoul, CLatentAction);
	
	bool CLASendSoul::HandleMessage(const SoulSenderMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos más de una petición simultánea
		if (msg._type != MessageType::SOUL_SENDER_REQUEST || _task != nullptr)
			return false;

		// Guardamos la informción del mensaje
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

		// Limpiamos el vector de almas
		_newSouls.clear();

		_numSoulsSent = 0;

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLASendSoul::OnRun(unsigned int msecs) {
		// Verificamos que tenemos tarea
		if (_task == nullptr)
			return LAStatus::FAIL;

		// Si no se pudieron crear las almas esperamos al siguiente tick
		if (!createSouls())
			return LAStatus::RUNNING;

		// Si no se pudieron enviar las almas esperamos al siguiente tick
		if (!sendSouls())
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}

	bool CLASendSoul::createSouls(){
		bool ret = true;

		// Creamos almas hasta completar las pedidas
		for (int i = _newSouls.size(); i < _numSouls; ++i){
			// Creamos alma
			CMap* map = Logic::CServer::getSingletonPtr()->getMap();
			CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

			if (!newSoul){
				assert(false && "Can´t create new soul");
				ret = false;
				break;
			}

			// La ubicamos en nuestra propia posición
			Vector3 newPosition = _entity->getPosition();
			newPosition.y = SOUL_ON_TILE_HEIGHT;
			PositionMessage m(newPosition);
			if (!m.Dispatch(*newSoul)){
				assert(false && "Can´t set soul on initial position");
				ret = false;
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(newSoul);
				break;
			}

			_newSouls.push_back(newSoul);
		}

		return ret;
	}

	bool CLASendSoul::sendSouls(){
		bool ret = true;

		// Comenzamos el bucle por la última alma enviada
		for (unsigned int i = _numSoulsSent; i < _newSouls.size(); ++i){
			// Le asignamos la tarea
			SoulMessage m2(_task->clone());
			if (!m2.Dispatch(*_newSouls[i])){
				//std::cout << "Can´t assign task to soul" << std::endl;
				ret = false;
				break;
			}

			++_numSoulsSent;
		}

		return ret;
	}
}