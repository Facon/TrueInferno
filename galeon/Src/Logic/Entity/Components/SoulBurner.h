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
		CSoulBurner() {}
		virtual ~CSoulBurner() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		/** Devuelve el n�mero actual de almas a ser quemadas en esta entidad */
		int getCurrentSoulsToBurn();

		/** Devuelve el n�mero m�ximo de almas que pueden quemarse en cada ciclo */
		int getMaxSoulsToBurn();

		/** Incrementa el n�mero de almas a quemar asignadas pero a�n en camino. */
		void increaseAssignedSoulsToBurn();

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMSoulBurnerData>* getStateMachine(){
			return new AI::CSMSoulBurner(_entity);
		}

	};
	
	REG_FACTORY(CSoulBurner);

} // namespace Logic

#endif // SOUL_BURNER_