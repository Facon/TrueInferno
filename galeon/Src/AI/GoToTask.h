#ifndef GO_TO_TASK_H_
#define GO_TO_TASK_H_

#include "SoulTask.h"

namespace Logic{
	class CPlaceable;
}

namespace AI{
	class CGoToTask : public CSoulTask {
	public:
		CGoToTask(Logic::CPlaceable* target, Logic::SoulActionMessage msg) : _target(target), _msg(msg) {}

		virtual void executeTask();

	private:
		Logic::CPlaceable* _target;
		Logic::SoulActionMessage _msg;
	};
}

#endif //GO_TO_TASK_H_