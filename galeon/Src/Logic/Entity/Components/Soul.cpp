#include "Soul.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CSoul);
	IMP_FACTORY(CSoul);

	bool CSoul::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		//if (entityInfo->hasAttribute("life"))
		//	_life = entityInfo->getFloatAttribute("life");

		return true;

	} // spawn

	void CSoul::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

}