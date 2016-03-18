/*
#ifndef SM_EMPTY_TEMPLATE_
#define SM_EMPTY_TEMPLATE_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAGetTaskAndTarget.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {

	class CSMEmptyTemplate : public CStateMachine<CLatentAction, CSMEmptyTemplateData> {
	public:
		CSMEmptyTemplate(CEntity* entity) : CStateMachine(entity) {
			CSMEmptyTemplateData data;

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAGetTaskAndTarget(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMEmptyTemplate() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(XXXMessage);
	};
}

#endif // SM_EMPTY_TEMPLATE_
*/