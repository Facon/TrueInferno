#include "Furnace.h"
#include "Application/GaleonApplication.h"
#include "Application/GameState.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CFurnace);
	IMP_FACTORY(CFurnace);

	bool CFurnace::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CFurnace::tick(unsigned int msecs){
		IComponent::tick(msecs);
	} // tick

}