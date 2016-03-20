#include "ResourceProvider.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic {
	RTTI_ROOT_IMPL(CResourceProvider);
	IMP_FACTORY(CResourceProvider);

	bool CResourceProvider::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CResourceProvider::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic