#include "Soul.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CSoul);
	IMP_FACTORY(CSoul);

	bool CSoul::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;

	} // spawn

	void CSoul::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

	CPlaceable* CSoul::getBuilding(){
		return ((AI::CSMSoulData*)_currentStateMachine)->getBuilding();
	} // getBuilding

	void CSoul::setBuilding(CPlaceable *building){
		((AI::CSMSoulData*)_currentStateMachine)->setBuilding(building);
	} // setBuilding

	void CSoul::clearBuilding(){
		((AI::CSMSoulData*)_currentStateMachine)->clearBuilding();
	} // setBuilding

}