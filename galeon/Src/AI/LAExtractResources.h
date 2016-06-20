#ifndef LA_EXTRACT_RESOURCES_
#define LA_EXTRACT_RESOURCES_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMResourceExtractorData.h"
#include "Logic\ResourcesManager.h"

namespace AI {
	class CLAExtractResources : public CLatentAction {
		RTTI_DECL;

	public:
		CLAExtractResources(
			CEntity* entity, CSMResourceExtractorData& smData, 
			ResourceType resourceType, unsigned int maxExtractedQuantity, int extractParticlesDuration) :
			CLatentAction(entity), _smData(smData), 
			_resourceType(resourceType), _maxExtractedQuantity(maxExtractedQuantity), _extractParticlesDuration(extractParticlesDuration) {}

		virtual ~CLAExtractResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceExtractorData& _smData;

		const int _maxExtractedQuantity;

		ResourceType _resourceType;

		/** Duración (ms) de las partículas de extracción de recursos */
		int _extractParticlesDuration;
	};
}

#endif