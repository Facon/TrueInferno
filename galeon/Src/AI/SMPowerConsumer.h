#ifndef SM_POWER_CONSUMER_H_
#define SM_POWER_CONSUMER_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAGetTaskAndTarget.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {

	class CSMPowerConsumer : public CStateMachine<CLatentAction, CSMPowerConsumerData> {
	public:
		CSMPowerConsumer(CEntity* entity) : CStateMachine(entity) {
			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTaskAndTarget(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMPowerConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);
	};
}

#endif // SM_POWER_CONSUMER_H_