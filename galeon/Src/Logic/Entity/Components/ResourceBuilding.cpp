#include "ResourceBuilding.h"

namespace Logic {
	RTTI_ROOT_IMPL(CResourceBuilding);
	IMP_FACTORY(CResourceBuilding);

	bool CResourceBuilding::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceBuilding::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic