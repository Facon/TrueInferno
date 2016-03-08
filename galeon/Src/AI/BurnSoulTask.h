#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Components\Placeable.h"

namespace AI{

	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(CPlaceable* target) {
			_target = target;
		};
		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_target);
		}

		virtual bool execute() {
			// Notificamos al objetivo
			NumberMessage m(MessageType::FURNACE_BURN_SOULS, 1);
			return m.Dispatch(*_target->getEntity());
		};

	};

}

#endif