#ifndef SOUL_H_
#define SOUL_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMSoul.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CSoul : public CStateMachineExecutor<AI::CSMSoulData> {
		RTTI_DECL;
		DEC_FACTORY(CSoul);

	public:
		CSoul() {}
		virtual ~CSoul() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMSoulData>* getStateMachine(){
			return new AI::CSMSoul(_entity);
		}

	};
	
	REG_FACTORY(CSoul);

} // namespace Logic

#endif // SOUL_H_