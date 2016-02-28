#ifndef LA_SEND_SOUL_
#define LA_SEND_SOUL_

#include "StateMachine.h"
#include "Server.h"
#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI {
	class CLASendSoul : public CLatentAction {
	public:
		CLASendSoul(CEntity* entity) : CLatentAction(entity) {
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

		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;
		int _numSouls;

		bool createAndSendSouls();
	};

}

#endif