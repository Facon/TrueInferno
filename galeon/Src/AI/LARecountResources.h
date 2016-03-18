#ifndef LA_RECOUNT_RESOURCES_
#define LA_RECOUNT_RESOURCES_

#include "AI\LatentAction.h"
#include "AI\SMResourceTransformerData.h"

namespace AI {
	class CLARecountResources : public CLatentAction {
	public:
		CLARecountResources(CEntity* entity, CSMResourceTransformerData& smData, ResourceType resourceFrom) : CLatentAction(entity), _smData(smData), _resourceFrom(resourceFrom) {}

		virtual ~CLARecountResources() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceTransformerData _smData;
		
		const ResourceType _resourceFrom;

		bool _requestReceived;
	};
}

#endif // LA_RECOUNT_RESOURCES_