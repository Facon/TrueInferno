#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"

namespace AI{

	class CBurnTask : public CSoulTask {
	public:
		CBurnTask() {};
		virtual ~CBurnTask() {};

		virtual bool execute() {
			// TODO
			return false;
		};
	};

}

#endif