#ifndef LA_EXECUTE_SOUL_TASK_
#define LA_EXECUTE_SOUL_TASK_

#include "StateMachine.h"
#include "Server.h"
#include "Logic\Entity\Message.h"
#include "SoulTask.h"

#include <vector>

namespace AI {
	class CLAExecuteSoulTask : public CLatentAction {

	public:
		CLAExecuteSoulTask(CEntity* entity) : CLatentAction(entity) {
		}

		virtual ~CLAExecuteSoulTask() {}

	protected:
		LAStatus OnRun();

	private:
		CSoulTask* _task;

	};

}

#endif