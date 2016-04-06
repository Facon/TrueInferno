/*
#ifndef SM_EMPTY_TEMPLATE_H_
#define SM_EMPTY_TEMPLATE_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAXXX.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {

	class CSMEmptyTemplate : public CStateMachine<CLatentAction, CSMEmptyTemplateData> {
	public:
		CSMEmptyTemplate(CEntity* entity) : CStateMachine(entity) {
			int start = this->addNode(new CLAXXX(entity, _data));

			this->addEdge(start, stateX, new CConditionSuccess());
			this->addEdge(start, start, new CConditionFail());

			this->addEdge(stateX, stateY, new CConditionSuccess());
			this->addEdge(stateX, start, new CConditionFail());

			this->addEdge(stateY, stateX, new CConditionSuccess());
			this->addEdge(stateY, start, new CConditionFail());
			
			this->setInitialNode(start);
			this->resetExecution();
		}

		virtual ~CSMEmptyTemplate() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(XXXMessage);
	};
}

#endif // SM_EMPTY_TEMPLATE_H_
*/