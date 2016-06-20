#ifndef LA_BURN_SOULS_
#define LA_BURN_SOULS_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMSoulBurnerData.h"

namespace AI {
	class CLABurnSouls : public CLatentAction {
		RTTI_DECL;

	public:
		CLABurnSouls(CEntity* entity, CSMSoulBurnerData& smData, int burnParticlesDuration) :
			CLatentAction(entity), _smData(smData), _burnParticlesDuration(burnParticlesDuration) {}

		virtual ~CLABurnSouls() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulBurnerData& _smData;

		/** Flags para se�alar que el incremento de coke y crude ya est� hecho */
		bool _cokeIncreased;
		bool _crudeIncreased;

		/** Suma de las cantidades de coke y crude de cada alma a quemar */
		int _totalCokeToIncrease;
		int _totalCrudeToIncrease;

		/** Duraci�n (ms) de las part�culas de quemado de almas */
		int _burnParticlesDuration;
	};
}

#endif