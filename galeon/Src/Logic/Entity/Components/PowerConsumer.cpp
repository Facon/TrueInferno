#include "PowerConsumer.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

namespace Logic {
	RTTI_ROOT_IMPL(CPowerConsumer);
	IMP_FACTORY(CPowerConsumer);

	bool CPowerConsumer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CPowerConsumer::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic