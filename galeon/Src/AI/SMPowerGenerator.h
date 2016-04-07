#ifndef SM_POWER_GENERATOR_H_
#define SM_POWER_GENERATOR_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAWaitConsumerChange.h"
#include "AI\LACheckNewConsumption.h"
#include "AI\LAAnswerCostumer.h"
#include "AI\LAFillReserves.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {

	class CSMPowerGenerator : public CStateMachine<CLatentAction, CSMPowerGeneratorData> {
	public:
		CSMPowerGenerator(CEntity* entity) : CStateMachine(entity) {
			int waitConsumer = this->addNode(new CLAWaitConsumerChange(entity, _data));
			int checkNewConsumption = this->addNode(new CLACheckNewConsumption(entity, _data));
			int answerCostumer = this->addNode(new CLAAnswerCostumer(entity, _data));
			int fillReserves = this->addNode(new CLAFillReserves(entity, _data));

			this->addEdge(waitConsumer, checkNewConsumption, new CConditionSuccess());
			this->addEdge(waitConsumer, waitConsumer, new CConditionFail());

			this->addEdge(checkNewConsumption, answerCostumer, new CConditionSuccess());
			this->addEdge(checkNewConsumption, fillReserves, new CConditionFail());

			this->addEdge(answerCostumer, waitConsumer, new CConditionFinished());

			this->addEdge(fillReserves, answerCostumer, new CConditionSuccess());
			this->addEdge(fillReserves, waitConsumer, new CConditionFail());

			this->setInitialNode(waitConsumer);
			this->resetExecution();
		}

		virtual ~CSMPowerGenerator() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);
		SM_HANDLE_MESSAGE(ResourceMessage);
	};
}

#endif // SM_POWER_GENERATOR_H_