#ifndef LA_ANSWER_COSTUMER_H_
#define LA_ANSWER_COSTUMER_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLAAnswerConsumer : public CLatentAction {

	public:
		CLAAnswerConsumer(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData), _consumerNotified(false) {}

		virtual ~CLAAnswerConsumer() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMPowerGeneratorData& _smData;

		// Flag a true cuando el consumidor ya ha sido notificado
		bool _consumerNotified;
	};
}

#endif // LA_ANSWER_COSTUMER_H_