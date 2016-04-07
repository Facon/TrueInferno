#ifndef LA_ANSWER_COSTUMER_H_
#define LA_ANSWER_COSTUMER_H_

#include "AI\LatentAction.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	class CLAAnswerCostumer : public CLatentAction {

	public:
		CLAAnswerCostumer(CEntity* entity, CSMPowerGeneratorData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAAnswerCostumer() {}

	protected:
		LAStatus OnStart();

	private:
		CSMPowerGeneratorData& _smData;

	};
}

#endif // LA_ANSWER_COSTUMER_H_