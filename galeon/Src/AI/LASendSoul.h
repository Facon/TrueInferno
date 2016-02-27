#ifndef LA_SEND_SOUL_
#define LA_SEND_SOUL_

#include "StateMachine.h"
#include "Server.h"
#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI {
	class CLASendSoul : public CLatentAction {
	public:
		CLASendSoul() {
			_task = nullptr;
		}

		virtual ~CLASendSoul() {
			if (_task){
				delete _task;
				_task = nullptr;
			}
		}

		virtual bool HandleMessage(const SoulSenderMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun();

	private:
		const float SOUL_ON_TILE_HEIGHT = 2.0;

		CSoulTask* _task;
		int _numSouls;

		bool createAndSendSoul();
	};

}

#endif