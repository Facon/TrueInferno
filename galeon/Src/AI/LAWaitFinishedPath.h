#ifndef LA_WAIT_FINISHED_PATH_
#define LA_WAIT_FINISHED_PATH_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"

#include <vector>

namespace AI {
	class CLAWaitFinishedPath : public CLatentAction {
		RTTI_DECL;

	public:
		CLAWaitFinishedPath(CEntity* entity) : CLatentAction(entity) {
		}

		virtual ~CLAWaitFinishedPath() {}

		virtual bool HandleMessage(const SoulMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun();

	};
}

#endif