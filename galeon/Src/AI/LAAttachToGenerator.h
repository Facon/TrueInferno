#ifndef LA_ATTACH_TO_GENERATOR_H_
#define LA_ATTACH_TO_GENERATOR_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	class CLAAttachToGenerator : public CLatentAction {
		RTTI_DECL;

	public:
		CLAAttachToGenerator(CEntity* entity, CSMPowerConsumerData& smData, int consumptionUnits, int consumptionPeriod) :
			CLatentAction(entity), 
			_smData(smData), 
			_received(false),
			_consumptionUnits(consumptionUnits),
			_consumptionPeriod(consumptionPeriod) {}

		virtual ~CLAAttachToGenerator() {}

		virtual bool HandleMessage(const PowerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerConsumerData& _smData;

		// Flag a true si ya hemos recibido respuesta en este tick
		bool _received;

		// Unidades de consumo por periodo
		int _consumptionUnits;

		/** Periodo (ms) con que se consumen recursos */
		int _consumptionPeriod;
	};
}

#endif // LA_ATTACH_TO_GENERATOR_H_