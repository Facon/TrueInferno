#include "HellQuarters.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CHellQuarters);
	IMP_FACTORY(CHellQuarters);

	bool CHellQuarters::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CHellQuarters::tick(unsigned int msecs){
		// Tickeamos la FSM
		CStateMachineExecutor::tick(msecs);

	} // tick

} // namespace Logic