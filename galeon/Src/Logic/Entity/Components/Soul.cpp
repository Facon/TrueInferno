#include "Soul.h"

namespace Logic
{
	RTTI_ROOT_IMPL(Soul);
	IMP_FACTORY(Soul);

	bool Soul::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		//if (entityInfo->hasAttribute("life"))
		//	_life = entityInfo->getFloatAttribute("life");

		return true;

	} // spawn
}