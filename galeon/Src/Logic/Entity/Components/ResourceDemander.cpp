#include "ResourceDemander.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic {
	RTTI_ROOT_IMPL(CResourceDemander);
	IMP_FACTORY(CResourceDemander);

	bool CResourceDemander::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceDemander::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic