#ifndef SM_HELL_QUARTERS_
#define SM_HELL_QUARTERS_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "LAGetTaskAndTarget.h"

namespace AI {
	class CSMHellQuartersData {
	public:
		CSMHellQuartersData() {}
		virtual ~CSMHellQuartersData() {}
	};

	class CSMHellQuarters : public CStateMachine<CLatentAction, CSMHellQuartersData> {
	public:
		CSMHellQuarters(CEntity* entity) : CStateMachine(entity) {
			CSMHellQuartersData data;

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTaskAndTarget(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMHellQuarters() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(HellQuartersMessage);
	};
}

#endif // SM_HELL_QUARTERS_