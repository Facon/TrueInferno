#include "LASendSoul.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/Billboard.h"
#include "Graphics\Entity.h"

namespace AI {
	RTTI_IMPL(CLASendSoul, CLatentAction);
	
	bool CLASendSoul::HandleMessage(const SoulSenderMessage& msg) {
		// Rechazamos lo que no sean peticiones. No aceptamos m�s de una petici�n simult�nea
		if (msg._type != MessageType::SOUL_SENDER_REQUEST || _task != nullptr)
			return false;

		// Guardamos la informci�n del mensaje
		
		if (_task)
		{
			delete _task;
		}

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

		// Suspendemos la LA hasta que llegue el mensaje con la petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLASendSoul::OnRun(unsigned int msecs) {
		// Acumulamos el tiempo transcurrido desde el �ltimo tick
		_timeSinceLastSoulSent += msecs;

		// Verificamos que tenemos tarea
		if (_task == nullptr)
			return LAStatus::FAIL;

		// Verificamos que se puede enviar un nuevo alma...
		if (_timeSinceLastSoulSent < _timeBetweenSouls)
			return LAStatus::RUNNING;

		// ...y reseteamos el tiempo acumulado
		_timeSinceLastSoulSent = 0;

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
				assert(false && "Can�t create new soul");
				ret = false;
				break;
			}

			// La ubicamos en nuestra propia posici�n
			Vector3 newPosition = _entity->getPosition();
			newPosition.y = SOUL_ON_TILE_HEIGHT;
			PositionMessage m(newPosition);

			if (!m.Dispatch(*newSoul)){
				assert(false && "Can�t set soul on initial position");
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

		// Comenzamos el bucle por la �ltima alma enviada
		for (unsigned int i = _numSoulsSent; i < _newSouls.size(); ++i){
			// Le asignamos la tarea
			CSoulTask* clone = _task->clone();
			
			// @todo
			// We add the resource icon that souls transports
			//IconMessage im(MessageType::ICON_ADD, _newSouls[i]->getComponent<Logic::Billboard>()->getResourceIcon(clone->));
			//im.Dispatch(*_entity);

			SoulMessage m2(clone);
			if (!m2.Dispatch(*_newSouls[i])){
				//std::cout << "Can�t assign task to soul" << std::endl;
				
				ret = false;
				break;
			}

			++_numSoulsSent;
		}

		return ret;
	}
}