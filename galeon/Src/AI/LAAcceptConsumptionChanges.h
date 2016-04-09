#ifndef LA_ACCEPT_CONSUMPTION_CHANGES_H_
#define LA_ACCEPT_CONSUMPTION_CHANGES_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	class CLAAcceptConsumptionChanges : public CLatentAction {
	public:
		CLAAcceptConsumptionChanges(CEntity* entity, CSMResourceConsumerData& smData, ResourceType resourceType) :
		CLatentAction(entity), _smData(smData), _resourceType(resourceType) {}

		virtual ~CLAAcceptConsumptionChanges() {}

		virtual bool HandleMessage(const ConsumptionMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		ResourceType _resourceType;
	};
}

#endif // LA_ACCEPT_CONSUMPTION_CHANGES_H_