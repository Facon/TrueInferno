#include "PathFinder.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>
#include "AI/SMPathFinder.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPathFinder);
	IMP_FACTORY(CPathFinder);

	CPathFinder::CPathFinder()  {
	}

	bool CPathFinder::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		std::cout << "CPathFinder spawned!" << std::endl;
		return true;
	} // spawn

	void CPathFinder::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

	AI::CStateMachine<AI::CLatentAction>* CPathFinder::getStateMachine(){
		return new AI::CSMPathFinder<AI::CLatentAction>(_entity);
	}


} // namespace Logic