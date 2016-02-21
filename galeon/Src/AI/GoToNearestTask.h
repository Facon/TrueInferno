#ifndef GO_TO_NEAREST_TASK_H_
#define GO_TO_NEAREST_TASK_H_

#include "SoulTask.h"

namespace Logic{
	enum BuildingType;
}

namespace AI{
	class CGoToNearestTask : public CSoulTask {
	public:
		CGoToNearestTask(Logic::BuildingType target) : _target(target) {}

		virtual void executeTask();

	private:
		Logic::BuildingType _target;
	};
}

#endif //GO_TO_NEAREST_TASK_H_