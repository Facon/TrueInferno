#include "SoulSender.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	RTTI_ROOT_IMPL(CSoulSender);
	IMP_FACTORY(CSoulSender);

	CSoulSender::CSoulSender() : IComponent() {

	}

	bool CSoulSender::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		std::cout << "CSoulSender spawned!" << std::endl;
		return true;
	} // spawn

	void CSoulSender::tick(unsigned int msecs){

	} // tick

} // namespace Logic