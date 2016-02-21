#include "HellQuarters.h"


#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/BuildingManager.h"
#include "AI/Server.h"
#include "AI/SoulTask.h"
#include "AI/GoToNearestTask.h"
#include "AI/GoToTask.h"
#include <iostream>
#include <cassert>

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
			std::cout << "HellQuarters is busy" << std::endl;
			return false;
		}

		switch (msg._action){

		case HellQuartersAction::BurnSoul:

		case HellQuartersAction::SendSoulToWork:{
			_actionRequested = new HellQuartersActionMessage(msg);
			_hellQuartersActionState = HellQuartersActionState::Requested;

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
		if (_hellQuartersActionState != HellQuartersActionState::WaitingTask){
			assert(false && "HellQuarters is not expecting message");
			return false;
		}

		if (msg._status){
			_hellQuartersActionState = HellQuartersActionState::Success;
		}
		else{
			_hellQuartersActionState = HellQuartersActionState::Fail;
		}
	}

	void CHellQuarters::tickActions(unsigned int msecs){
		switch (_hellQuartersActionState){
		// En caso de estar parados, no se hace nada
		case HellQuartersActionState::Idle:{
			break;
		}

		case HellQuartersActionState::Requested:{
			// Reservamos las almas
			_numAvailableSouls -= _actionRequested->_numSouls;

			if (_numAvailableSouls < 0){
				std::cout << "There are no available souls" << std::endl;
				_hellQuartersActionState = HellQuartersActionState::Fail;
				return;
			}

			AI::CSoulTask task;
			switch (_actionRequested->_action){

			case HellQuartersAction::BurnSoul:{
				task = AI::CGoToNearestTask(BuildingType::Furnace);
				break;
			}

			case HellQuartersAction::SendSoulToWork:{
				task = AI::CGoToTask(CBuildingManager::getSingletonPtr()->getRandomBuilding());
				break;
			}

			default:
				break;
			}

			// Enviamos la solicitud de ejecución de tarea
			SoulSenderRequestMessage m(task, _actionRequested->_numSouls);

			if (m.Dispatch(*_entity)){
				_hellQuartersActionState = HellQuartersActionState::WaitingTask;
			}
			else{
				_hellQuartersActionState = HellQuartersActionState::Fail;
			}

			break;
		}

		case HellQuartersActionState::WaitingTask:{
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