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

}