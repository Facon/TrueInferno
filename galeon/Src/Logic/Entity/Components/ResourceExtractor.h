#ifndef RESOURCE_EXTRACTOR_H_
#define RESOURCE_EXTRACTOR_H_

#include "StateMachineExecutor.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"
#include "AI/SMResourceExtractorData.h"
#include "AI/SMResourceExtractor.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceExtractor : public CStateMachineExecutor<AI::CSMResourceExtractorData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceExtractor);

	public:
		CResourceExtractor() {}
		virtual ~CResourceExtractor() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceExtractorData>* getStateMachine(){
			return new AI::CSMResourceExtractor(_entity);
		}

	};
	
	REG_FACTORY(CResourceExtractor);

} // namespace Logic

#endif // RESOURCE_EXTRACTOR_H_