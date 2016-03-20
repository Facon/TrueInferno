#ifndef LA_EXECUTE_RESOURCE_PROVIDE_TASKS_H_
#define LA_EXECUTE_RESOURCE_PROVIDE_TASKS_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	class CLAExecuteResourceProvideTasks : public CLatentAction {
	public:
		CLAExecuteResourceProvideTasks(CEntity* entity, CSMResourceProviderData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAExecuteResourceProvideTasks() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceProviderData& _smData;
	};
}

#endif // LA_EXECUTE_RESOURCE_PROVIDE_TASKS_H_