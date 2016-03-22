#ifndef LA_EXECUTE_LOGISTICS_TASKS_
#define LA_EXECUTE_LOGISTICS_TASKS_

#include "AI\LatentAction.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {
	class CLAExecuteResourceDemandTasks : public CLatentAction {
	public:
		CLAExecuteResourceDemandTasks(CEntity* entity, CSMResourceDemanderData& smData) : CLatentAction(entity), _smData(smData), _nextProvider(0) {}

		virtual ~CLAExecuteResourceDemandTasks() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceDemanderData& _smData;

		// Índice para recorrer el vector de proveedores entre distintos ticks
		int _nextProvider = 0;
	};
}

#endif