#include "WorkBuilding.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Message.h"
#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CWorkBuilding);
	IMP_FACTORY(CWorkBuilding);

	CWorkBuilding::CWorkBuilding() : IComponent() {
	}

	bool CWorkBuilding::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		assert(entityInfo->hasAttribute("minWorkers") && "minWorkers is not defined");
		_minWorkers = entityInfo->getIntAttribute("minWorkers");

		assert(entityInfo->hasAttribute("maxWorkers") && "maxWorkers is not defined");
		_maxWorkers = entityInfo->getIntAttribute("maxWorkers");
		
		_activeWorkers = 0;
		_assignedWorkers = 0;

		_changeActive = 0;
		_changeAssigned = 0;

		return true;
	} // spawn

	void CWorkBuilding::tick(unsigned int msecs){
		// Si había pendiente una orden de cambiar el número de trabajadores activos, se procesa
		if (_changeActive != 0){
			_activeWorkers += _changeActive;

			//std::cout << "Current active workers=" << _activeWorkers << std::endl;

			_changeActive = 0;
		}

		// Si había pendiente una orden de cambiar el número de trabajadores asignados, se procesa
		if (_changeAssigned != 0){
			_assignedWorkers += _changeAssigned;

			//std::cout << "Current assigned workers=" << _assignedWorkers << std::endl;

			_changeAssigned = 0;
		}

	} // tick

	bool CWorkBuilding::isActive(){
		return _activeWorkers >= _minWorkers;
	}

	bool CWorkBuilding::HandleMessage(const WorkerMessage& msg){
		switch(msg._type){
		case TMessage::WORKER_ACTIVATED:{
			_changeActive += msg._change;
			break;
		}

		case TMessage::WORKER_ASSIGNED:{
			_changeAssigned += msg._change;
			break;
		}

		default:{
			assert("Unmiplemented logic for message type" && false);
			return false;
		}
		}

		return true;
	}

} // namespace Logic