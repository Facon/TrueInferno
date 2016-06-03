#include "SoulBurner.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CSoulBurner);
	IMP_FACTORY(CSoulBurner);

	bool CSoulBurner::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CSoulBurner::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

	int CSoulBurner::getVirtualSoulsToBurn(){
		return ((AI::CSMSoulBurner*)_currentStateMachine)->getVirtualSoulsToBurn();
		//return _data.getVirtualNumSoulsToBurn();
	}

	int CSoulBurner::getCurrentSoulsToBurn(){
		return ((AI::CSMSoulBurner*)_currentStateMachine)->getCurrentSoulsToBurn();
		//return _data.getNumSoulsToBurn();
	}

	int CSoulBurner::getMaxSoulsToBurn(){
		return ((AI::CSMSoulBurner*)_currentStateMachine)->getMaxSoulsToBurn();
		//return _maxSoulsPerCycle;
	}

	void CSoulBurner::increaseAssignedSoulsToBurn()
	{
		((AI::CSMSoulBurner*)_currentStateMachine)->increaseAssignedSoulsToBurn();
		//_data.increaseAssignedSoulsToBurn()
	}

}