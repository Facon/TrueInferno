#ifndef LA_RESERVE_RESOURCES_TO_CONSUME_H_
#define LA_RESERVE_RESOURCES_TO_CONSUME_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	class CLAReserveResourcesToConsume : public CLatentAction {
		RTTI_DECL;

	public:
		CLAReserveResourcesToConsume(CEntity* entity, CSMResourceConsumerData& smData, ResourceType consumedResource) : 
			CLatentAction(entity), _smData(smData), _received(false), _consumedResource(consumedResource) {}

		virtual ~CLAReserveResourcesToConsume() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		bool _received;

		ResourceType _consumedResource;
	};
}

#endif // LA_RESERVE_RESOURCES_TO_CONSUME_H_