#ifndef LA_STOPPED_RESOURCE_CONSUMER_H_
#define LA_STOPPED_RESOURCE_CONSUMER_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	class CLAStoppedResourceConsumer : public CLatentAction {
		RTTI_DECL;

	public:
		CLAStoppedResourceConsumer(CEntity* entity, ResourceType consumedResource) : CLatentAction(entity), _consumedResource(consumedResource) {}

		virtual ~CLAStoppedResourceConsumer() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		/** Tipo de recurso que se consume */
		ResourceType _consumedResource;
	};
}

#endif // LA_STOPPED_RESOURCE_CONSUMER_H_