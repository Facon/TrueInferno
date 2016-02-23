#ifndef SOUL_TASK_H_
#define SOUL_TASK_H_

#include "Logic\Entity\Message.h"
#include "Logic\Entity\Entity.h"

namespace AI{
	class CSoulTask {
		virtual bool executeTask();
	};

	class CSendMessageTask : public CSoulTask{
	public:
		CSendMessageTask(Logic::CEntity *target, const Logic::SoulActionMessage& message) : _target(target), _message(message) {}

		Logic::CEntity *_target;
		Logic::SoulActionMessage _message;

		bool CSendMessageTask::executeTask() { return _message.Dispatch(*_target); };
	};
}

#endif