#include "PowerGenerator.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPowerGenerator);
	IMP_FACTORY(CPowerGenerator);

	bool CPowerGenerator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CPowerGenerator::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

	int CPowerGenerator::getConsumption(){
		return _currentStateMachine->getSharedData().getTotalConsumption();
	}

	int CPowerGenerator::getCurrentReserves(){
		return _currentStateMachine->getSharedData().getCurrentCoke();
	}

} // namespace Logic