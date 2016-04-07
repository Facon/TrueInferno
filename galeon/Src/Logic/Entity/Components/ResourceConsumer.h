#ifndef RESOURCE_CONSUMER_H_
#define RESOURCE_CONSUMER_H_

#include "StateMachineExecutor.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"
#include "AI/SMResourceConsumerData.h"
#include "AI/SMResourceConsumer.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceConsumer : public CStateMachineExecutor<AI::CSMResourceConsumerData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceConsumer);

	public:
		CResourceConsumer() {}
		virtual ~CResourceConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceConsumerData>* getStateMachine(){
			return new AI::CSMResourceConsumer(_entity);
		}

	};
	
	REG_FACTORY(CResourceConsumer);

} // namespace Logic

#endif // RESOURCE_CONSUMER_H_