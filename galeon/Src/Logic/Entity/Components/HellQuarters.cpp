#include "HellQuarters.h"


#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/BuildingManager.h"
#include "AI/Server.h"
#include "AI/SoulTask.h"
#include <iostream>
#include <cassert>
#include "Logic/Entity/Components/Placeable.h"

namespace Logic {
	RTTI_ROOT_IMPL(CHellQuarters);
	IMP_FACTORY(CHellQuarters);

	CHellQuarters::CHellQuarters() : IComponent() {
	}

	bool CHellQuarters::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		_timeSinceLastSpawn = 0;

		assert(entityInfo->hasAttribute("numInitialSouls") && "numInitialSouls is not defined");
		_numAvailableSouls = entityInfo->getIntAttribute("numInitialSouls");

		assert(entityInfo->hasAttribute("secondsBetweenSpawns") && "secondsBetweenSpawns is not defined");
		_timeBetweenSpawns = 1000 * entityInfo->getIntAttribute("secondsBetweenSpawns");

		assert(entityInfo->hasAttribute("numSpawnedSouls") && "numSpawnedSouls is not defined");
		_numSpawnedSouls = entityInfo->getIntAttribute("numSpawnedSouls");

		_hellQuartersActionState = HellQuartersActionState::Idle;
		_actionRequested = nullptr;

		return true;
	} // spawn

	void CHellQuarters::tick(unsigned int msecs){
		tickSpawnSouls(msecs);

		tickActions(msecs);
	} // tick

	void CHellQuarters::tickSpawnSouls(unsigned int msecs){
		// Chequeamos si ha pasado tiempo suficiente
		_timeSinceLastSpawn += msecs;
		if (_timeSinceLastSpawn >= _timeBetweenSpawns){
			_timeSinceLastSpawn -= _timeBetweenSpawns;
			_numAvailableSouls += _numSpawnedSouls;

			std::cout << "available souls = " << _numAvailableSouls << std::endl;
			
			// TODO ¿Reproducimos algún sonido o animación de almas nuevas?
		}
	}

	bool CHellQuarters::HandleMessage(const HellQuartersActionMessage& msg){
		if (_hellQuartersActionState != HellQuartersActionState::Idle){
			assert(false && "HellQuarters is not expecting this message");
			return false;
		}

		switch (msg._action){

		case TMessage::HellQuartersAction::SendSoulToBurn:

		case TMessage::HellQuartersAction::SendSoulToWork:{
			_actionRequested = new HellQuartersActionMessage(msg);
			_hellQuartersActionState = HellQuartersActionState::ActionRequested;

			return true;
			break;
		}
			
		default:{
			assert(false && "Unimplemented HellQuarters action");
			return false;
		}
		}
	}

	bool CHellQuarters::HandleMessage(const SoulSenderResponseMessage& msg){
		if (_hellQuartersActionState != HellQuartersActionState::WaitingTaskStart){
			assert(false && "HellQuarters is not expecting this message");
			return false;
		}

		if (msg._status){
			_hellQuartersActionState = HellQuartersActionState::Success;
		}
		else{
			_hellQuartersActionState = HellQuartersActionState::Fail;
		}
	}

	bool CHellQuarters::HandleMessage(const WalkSoulPathMessage& msg){
		// Nos aseguramos que estamos recibiendo una respuesta y que estábamos en estado de esperarla
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH || _hellQuartersActionState != HellQuartersActionState::WaitingPath){
			assert(false && "HellQuarters is not expecting this message");
			return false;
		}

		// Guardamos la ruta devuelta. Puede ser NULL si no se encontró ruta al destino solicitado
		_pathReceived = msg._path;

		if (msg._path)
			// Cambiamos al estado de path recibido
			_hellQuartersActionState = HellQuartersActionState::PathReceived;
		else
			_hellQuartersActionState = HellQuartersActionState::Fail;

		return true;
	}

	void CHellQuarters::tickActions(unsigned int msecs){
		switch (_hellQuartersActionState){

		// En caso de estar parados, no se hace nada
		case HellQuartersActionState::Idle:{
			break;
		}

		case HellQuartersActionState::ActionRequested:{
			// Reservamos las almas
			_numAvailableSouls -= _actionRequested->_numSouls;

			// Comprobamos que hay almas suficientes
			if (_numAvailableSouls < 0){
				std::cout << "There are no available souls" << std::endl;
				_hellQuartersActionState = HellQuartersActionState::Fail;
				return;
			}

			CPlaceable* target = nullptr;

			switch (_actionRequested->_action){
			case TMessage::HellQuartersAction::SendSoulToBurn:{
				target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::Furnace);
				_task = AI::CSendMessageTask(target->getEntity(), SoulActionMessage(_actionRequested->_numSouls, TMessage::SoulAction::BurnSoul));
				break;
			}

			case TMessage::HellQuartersAction::SendSoulToWork:{
				target = CBuildingManager::getSingletonPtr()->getRandomBuilding();
				_task = AI::CSendMessageTask(target->getEntity(), SoulActionMessage(_actionRequested->_numSouls, TMessage::SoulAction::WorkSoul));
				break;
			}

			default:
				assert(false && "Unimplemented HellQuarters action");
				_hellQuartersActionState = HellQuartersActionState::Fail;
				return;
			}

			// Enviamos un mensaje para obtener ruta hasta el objetivo
			WalkSoulPathMessage message(target);

			// Si nadie atendió al mensaje
			if (!message.Dispatch(*this->getEntity())){
				std::cout << "No one answered the REQUEST_WALK_SOUL_PATH message" << std::endl;
				_hellQuartersActionState = HellQuartersActionState::Fail;
				return;
			}

			// Nos ponemos a esperar a la ruta
			_hellQuartersActionState = HellQuartersActionState::WaitingPath;
			break;
		}

		case HellQuartersActionState::WaitingPath:{
			break;
		}

		case HellQuartersActionState::PathReceived:{
			// Enviamos la solicitud de ejecución de tarea
			SoulSenderRequestMessage m(_pathReceived, _task);

			if (m.Dispatch(*_entity)){
				_hellQuartersActionState = HellQuartersActionState::WaitingTaskStart;
			}
			else{
				_hellQuartersActionState = HellQuartersActionState::Fail;
			}
			
			break;
		}

		case HellQuartersActionState::WaitingTaskStart:{
			break;
		}

		case HellQuartersActionState::Fail:{
			// Recuperamos las almas reservadas dado que no han podido usarse realmente
			_numAvailableSouls += _actionRequested->_numSouls;

			// Pasamos al estado de limpieza
			_hellQuartersActionState = HellQuartersActionState::Clean;

			break;
		}

		case HellQuartersActionState::Success:{
			// Pasamos al estado de limpieza
			_hellQuartersActionState = HellQuartersActionState::Clean;

			break;
		}

		case HellQuartersActionState::Clean:{
			// Liberamos memoria
			if (_actionRequested){
				delete(_actionRequested);
				_actionRequested = nullptr;
			}

			_hellQuartersActionState = HellQuartersActionState::Idle;

			break;
		}

		default:{
			assert(true && "Unimplemented logic for all states");

			break;
		}
		}
	}

} // namespace Logic