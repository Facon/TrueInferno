#ifndef LA_ACCEPT_CONSUMPTION_CHANGES_H_
#define LA_ACCEPT_CONSUMPTION_CHANGES_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceConsumerData.h"

namespace AI {
	class CLAAcceptConsumptionChanges : public CLatentAction {
		RTTI_DECL;

	public:
		CLAAcceptConsumptionChanges(CEntity* entity, CSMResourceConsumerData& smData, ResourceType resourceType, int normalizedConsumptionPeriod) :
			CLatentAction(entity), _smData(smData), _resourceType(resourceType), _normalizedConsumptionPeriod(normalizedConsumptionPeriod) {}

		virtual ~CLAAcceptConsumptionChanges() {}

		virtual bool HandleMessage(const ConsumptionMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceConsumerData& _smData;

		// Tipo de recurso consumido
		ResourceType _resourceType;

		// Periodo (ms) normalizado de consumición de recursos
		int _normalizedConsumptionPeriod;
	};
}

#endif // LA_ACCEPT_CONSUMPTION_CHANGES_H_