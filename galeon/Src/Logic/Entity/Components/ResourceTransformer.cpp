#include "ResourceTransformer.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceTransformer);
	IMP_FACTORY(CResourceTransformer);

	bool CResourceTransformer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceTransformer::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

}