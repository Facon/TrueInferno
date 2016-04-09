#ifndef LA_STOP_CONSUMING_RESOURCES_H_
#define LA_STOP_CONSUMING_RESOURCES_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	class CLAStopConsumingResources : public CLatentAction {
	public:
		CLAStopConsumingResources(CEntity* entity, CSMResourceConsumerData& smData, ResourceType resourceType) :
			CLatentAction(entity), _smData(smData), _resourceType(resourceType) {}

		virtual ~CLAStopConsumingResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		ResourceType _resourceType;
	};
}

#endif // LA_STOP_CONSUMING_RESOURCES_H_