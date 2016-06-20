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
		CLAConsumeResources(CEntity* entity, CSMResourceConsumerData& smData, ResourceType resourceType, int consumptionParticlesDuration) :
			CLatentAction(entity), _smData(smData), _resourceType(resourceType), _consumptionParticlesDuration(consumptionParticlesDuration) {}

		virtual ~CLAConsumeResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		ResourceType _resourceType;

		/** Duración (ms) de las partículas de consumo de recursos */
		int _consumptionParticlesDuration;
	};
}

#endif // LA_CONSUME_RESOURCES_H_