#ifndef LA_WAIT_TASK_REQUEST_PATH_
#define LA_WAIT_TASK_REQUEST_PATH_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAWaitTaskAndRequestPath : public CLatentAction {
	public:
		CLAWaitTaskAndRequestPath(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData), _taskStarted(false) {}

		virtual ~CLAWaitTaskAndRequestPath() {}

		virtual bool HandleMessage(const SoulMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;

		/** Solicita una ruta al objetivo de la petición almacenada */
		bool requestPath();

		/** Flag a true cuando la tarea ya ha posido ser empezada correctamente */
		bool _taskStarted;
	};
}

#endif