#include "ResourceBuilding.h"

namespace Logic {
	RTTI_ROOT_IMPL(CResourceBuilding);
	IMP_FACTORY(CResourceBuilding);

	bool CResourceBuilding::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceBuilding::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

	int CResourceBuilding::getStoredResources(ResourceType type){
		return _currentStateMachine->getSharedData().getStoredResources(type);
	}

	int CResourceBuilding::getAvailableResources(ResourceType type){
		return _currentStateMachine->getSharedData().getAvailableResources(type);
	}

	int CResourceBuilding::getMaxResources(ResourceType type){
		return _currentStateMachine->getSharedData().getMaxResources(type);
	}

	bool CResourceBuilding::changeStoredResources(ResourceType type, int quantity){
		return _currentStateMachine->getSharedData().changeStoredResources(type, quantity);
	}

	bool CResourceBuilding::reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved){
		return _currentStateMachine->getSharedData().reserveResources(type, quantity, allowPartial, finallyReserved);
	}

	bool CResourceBuilding::freeReservedResources(ResourceType type, int quantity){
		return _currentStateMachine->getSharedData().freeReservedResources(type, quantity);
	}

	bool CResourceBuilding::claimReservedResources(ResourceType type, int quantity){
		return _currentStateMachine->getSharedData().claimReservedResources(type, quantity);
	}

} // namespace Logic