#ifndef RESOURCE_BUILDING_H_
#define RESOURCE_BUILDING_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceBuilding.h"

namespace Logic {
	class CResourceBuilding : public CStateMachineExecutor<AI::CSMResourceBuildingData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceBuilding);

	public:
		CResourceBuilding() {}
		
		virtual ~CResourceBuilding() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceBuildingData>* getStateMachine(){
			return new AI::CSMResourceBuilding(_entity);
		}
	};
	
	REG_FACTORY(CResourceBuilding);

} // namespace Logic

#endif // RESOURCE_BUILDING_H_