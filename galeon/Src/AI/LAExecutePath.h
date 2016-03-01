#ifndef LA_EXECUTE_PATH_
#define LA_EXECUTE_PATH_

#include "StateMachine.h"
#include "Server.h"
#include "Logic\Entity\Message.h"

#include <vector>

namespace AI {
	class CLAExecutePath : public CLatentAction {
	public:
		CLAExecutePath(CEntity* entity) : CLatentAction(entity) {
		}

		virtual ~CLAExecutePath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun();

	private:
		std::vector<Vector3>* _path;

		bool CLAExecutePath::sendPath();
	};

}

#endif