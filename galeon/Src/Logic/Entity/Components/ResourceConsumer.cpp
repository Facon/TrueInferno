#include "ResourceConsumer.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceConsumer);
	IMP_FACTORY(CResourceConsumer);

	bool CResourceConsumer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceConsumer::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

}