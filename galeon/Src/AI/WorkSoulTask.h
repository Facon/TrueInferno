#ifndef WORK_SOUL_TASK_H_
#define WORK_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Components\Placeable.h"

namespace AI{

	class CWorkTask : public CSoulTask {
	public:
		CWorkTask(CPlaceable* target) : _target(target) {};
		virtual ~CWorkTask() {};

		virtual bool execute() {
			// TODO
			return false;
		};

	private:
		CPlaceable* _target;
	};

}

#endif