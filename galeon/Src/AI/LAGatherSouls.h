#ifndef LA_GATHER_SOULS_
#define LA_GATHER_SOULS_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMSoulBurnerData.h"

namespace AI {
	/**
	Representa la acción de acumular almas durante un tiempo determinado y hasta una cantidad determinada
	*/
	class CLAGatherSouls : public CLatentAction {
		RTTI_DECL;

	public:
		CLAGatherSouls(CEntity* entity, CSMSoulBurnerData& smData, unsigned int burnPeriod, unsigned int maxSoulsPerCycle) : 
			CLatentAction(entity), _smData(smData), _burnPeriod(burnPeriod), _maxSoulsPerCycle(maxSoulsPerCycle) {}

		virtual ~CLAGatherSouls() {}

		bool HandleMessage(const SoulBurnMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulBurnerData& _smData;

		/** Tiempo transcurrido desde el último ciclo */
		unsigned int _elapsedTime;

		/** Periodo (ms) con que se queman cíclicamente las almas */
		unsigned const int _burnPeriod;

		/** Número máximo de almas que se queman en cada ciclo */
		unsigned const int _maxSoulsPerCycle;
	};

}

#endif