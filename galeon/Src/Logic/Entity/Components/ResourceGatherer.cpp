#include "ResourceGatherer.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceGatherer);
	IMP_FACTORY(CResourceGatherer);

	bool CResourceGatherer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceGatherer::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

}