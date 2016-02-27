#include "SoulSender.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	RTTI_ROOT_IMPL(CSoulSender);
	IMP_FACTORY(CSoulSender);

	bool CSoulSender::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// Garantizamos que se llama al padre
		if (!CStateMachineExecutor::spawn(entity, map, entityInfo))
			return false;

		std::cout << "CSoulSender spawned!" << std::endl;
		return true;
	} // spawn

	void CSoulSender::tick(unsigned int msecs){
		CStateMachineExecutor::tick(msecs);
	} // tick

} // namespace Logic