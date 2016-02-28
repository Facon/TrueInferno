#ifndef SOUL_TASK_H_
#define SOUL_TASK_H_

namespace AI{
	class CSoulTask {
	public:
		virtual bool execute() = 0;
		virtual CSoulTask* clone() = 0;
	};
}

#endif