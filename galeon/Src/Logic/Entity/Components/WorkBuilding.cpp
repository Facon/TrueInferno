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
		
		_currentWorkers = 0;

		_changeNumWorkers = 0;

		return true;
	} // spawn

	void CWorkBuilding::tick(unsigned int msecs){
		// Si había pendiente una orden de cambiar el número de trabajadores se procesa
		if (_changeNumWorkers != 0){
			_currentWorkers += _changeNumWorkers;

			std::cout << "Current workers=" << _currentWorkers << std::endl;

			_changeNumWorkers = 0;
		}

	} // tick

	bool CWorkBuilding::isActive(){
		return _currentWorkers >= _minWorkers;
	}

	bool CWorkBuilding::HandleMessage(const WorkerMessage& msg){
		_changeNumWorkers += msg._numWorkers;
		return true;
	}

} // namespace Logic