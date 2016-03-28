#ifndef LA_GET_TASK_TARGET_
#define LA_GET_TASK_TARGET_

#include "StateMachine.h"
#include "Server.h"
#include "SoulTask.h"
#include "WorkSoulTask.h"
#include "BurnSoulTask.h"
#include "Logic\BuildingManager.h"
#include "Logic\Entity\Message.h"

namespace AI {
	class CLAGetTaskAndTarget : public CLatentAction {
	public:
		CLAGetTaskAndTarget(CEntity* entity) : CLatentAction(entity) {
			_task = nullptr;
		}

		virtual ~CLAGetTaskAndTarget() {
			if (_task){
				delete _task;
				_task = nullptr;
			}
		}

		virtual bool HandleMessage(const HellQuartersMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSoulTask* _task;

		bool sendSoul();
	};

}

#endif