#ifndef RESOURCE_TRANSFORMER_
#define RESOURCE_TRANSFORMER_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceTransformer.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceTransformer : public CStateMachineExecutor<AI::CSMResourceTransformerData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceTransformer);

	public:
		CResourceTransformer() {}
		virtual ~CResourceTransformer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceTransformerData>* getStateMachine(){
			return new AI::CSMResourceTransformer(_entity);
		}

	};
	
	REG_FACTORY(CResourceTransformer);

} // namespace Logic

#endif // RESOURCE_TRANSFORMER_