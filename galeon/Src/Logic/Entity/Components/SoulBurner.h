#ifndef SOUL_BURNER_
#define SOUL_BURNER_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMSoulBurner.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CSoulBurner : public CStateMachineExecutor<AI::CSMSoulBurnerData> {
		RTTI_DECL;
		DEC_FACTORY(CSoulBurner);

	public:
		CSoulBurner() : _coke(0), _crude(0) {}
		virtual ~CSoulBurner() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMSoulBurnerData>* getStateMachine(){
			return new AI::CSMSoulBurner(_entity);
		}

	private:
		/** Cantidad de coke acumulado */
		int _coke;

		/** Cantidad de crude acumulado */
		int _crude;
	};
	
	REG_FACTORY(CSoulBurner);

} // namespace Logic

#endif // SOUL_H_