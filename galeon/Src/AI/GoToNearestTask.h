#ifndef GO_TO_NEAREST_TASK_H_
#define GO_TO_NEAREST_TASK_H_

#include "SoulTask.h"

namespace Logic{
	enum BuildingType;
}

namespace AI{
	class CGoToNearestTask : public CSoulTask {
	public:
		CGoToNearestTask(Logic::BuildingType target, Logic::SoulActionMessage msg) : _target(target), _msg(msg) {}

		virtual void executeTask();

	private:
		Logic::BuildingType _target;
		Logic::SoulActionMessage _msg;
	};
}

#endif //GO_TO_NEAREST_TASK_H_