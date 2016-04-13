#ifndef LA_CONSUME_RESOURCES_H_
#define LA_CONSUME_RESOURCES_H_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	class CLAConsumeResources : public CLatentAction {
		RTTI_DECL;

	public:
		CLAConsumeResources(CEntity* entity, CSMResourceConsumerData& smData, ResourceType resourceType) :
			CLatentAction(entity), _smData(smData), _resourceType(resourceType) {}

		virtual ~CLAConsumeResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		ResourceType _resourceType;
	};
}

#endif // LA_CONSUME_RESOURCES_H_