#ifndef RESOURCE_GATHERER_H_
#define RESOURCE_GATHERER_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceGatherer.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceGatherer : public CStateMachineExecutor<AI::CSMResourceGathererData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceGatherer);

	public:
		CResourceGatherer() {}
		virtual ~CResourceGatherer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceGathererData>* getStateMachine(){
			return new AI::CSMResourceGatherer(_entity);
		}

	};
	
	REG_FACTORY(CResourceGatherer);

} // namespace Logic

#endif // RESOURCE_GATHERER_H_