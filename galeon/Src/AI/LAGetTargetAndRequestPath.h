#ifndef LA_GET_TARGET_REQ_PATH_
#define LA_GET_TARGET_REQ_PATH_

#include "StateMachine.h"
#include "Server.h"
#include "Logic\Entity\Message.h"
#include "Logic\Entity\Components\Placeable.h"

namespace AI {
	class CLAGetTargetAndRequestPath : public CLatentAction {
	public:
		CLAGetTargetAndRequestPath(CEntity* entity) : CLatentAction(entity) {
			_target = nullptr;
		}

		virtual ~CLAGetTargetAndRequestPath() {}

		virtual bool HandleMessage(const SoulMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun();

	private:
		CPlaceable* _target;
		int _numSouls;

		bool requestPath();
	};

}

#endif