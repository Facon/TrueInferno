#ifndef SM_POWER_GENERATOR_H_
#define SM_POWER_GENERATOR_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAGetTaskAndTarget.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {

	class CSMPowerGenerator : public CStateMachine<CLatentAction, CSMPowerGeneratorData> {
	public:
		CSMPowerGenerator(CEntity* entity) : CStateMachine(entity) {
			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTaskAndTarget(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMPowerGenerator() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);
	};
}

#endif // SM_POWER_GENERATOR_H_