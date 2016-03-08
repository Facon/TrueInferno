#include "ResourceExtractor.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceExtractor);
	IMP_FACTORY(CResourceExtractor);

	bool CResourceExtractor::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceExtractor::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

}