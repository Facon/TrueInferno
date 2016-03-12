#ifndef LA_EXECUTE_LOGISTICS_TASKS_
#define LA_EXECUTE_LOGISTICS_TASKS_

#include "AI\LatentAction.h"
#include "AI\SMLogisticsData.h"

namespace AI {
	class CLAExecuteLogisticsTasks : public CLatentAction {
	public:
		CLAExecuteLogisticsTasks(CEntity* entity, CSMLogisticsData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLAExecuteLogisticsTasks() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMLogisticsData _smData;
	};
}

#endif