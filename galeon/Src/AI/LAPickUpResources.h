#ifndef LA_PICK_UP_RESOURCES_H_
#define LA_PICK_UP_RESOURCES_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	class CLAPickUpResources : public CLatentAction {
	public:
		CLAPickUpResources(CEntity* entity, CSMResourceProviderData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAPickUpResources() {}

		virtual bool HandleMessage(const ResourceMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceProviderData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_PICK_UP_RESOURCES_H_