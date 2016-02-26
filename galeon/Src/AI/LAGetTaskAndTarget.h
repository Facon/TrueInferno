#ifndef LA_GET_TASK_TARGET_
#define LA_GET_TASK_TARGET_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\SoulTask.h"
#include "AI\WorkSoulTask.h"
#include "AI\BurnSoulTask.h"
#include "Logic\BuildingManager.h"

namespace AI {
	class CLAGetTaskAndTarget : public CLatentAction {
	public:
		CLAGetTaskAndTarget() {
			_task = nullptr;
			_target = nullptr;
		}

		virtual ~CLAGetTaskAndTarget() {
			if (_task){
				delete _task;
				_task = nullptr;
			}
			_target = nullptr;
		}

		virtual bool HandleMessage(const HellQuartersMessage& msg);

	protected:
		virtual LAStatus OnStart();

		virtual LAStatus OnRun();

	private:
		CSoulTask* _task;
		CPlaceable* _target;

		bool sendSoul();
	};

}

#endif