#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Components\Placeable.h"

namespace AI{

	class CBurnTask : public CSoulTask {
	public:
		CBurnTask(CPlaceable* target) : _target(target) {};
		virtual ~CBurnTask() {};

		virtual bool execute() {
			// TODO
			return false;
		};

	private:
		CPlaceable* _target;
	};

}

#endif