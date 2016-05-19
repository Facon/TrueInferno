#ifndef LA_UPDATE_EXTRACTION_SPEED_H_
#define LA_UPDATE_EXTRACTION_SPEED_H_

#include "AI\LatentAction.h"
#include "AI\SMResourceExtractorData.h"

namespace AI {
	class CLAUpdateExtractionSpeed : public CLatentAction {
		RTTI_DECL;

	public:
		CLAUpdateExtractionSpeed(CEntity* entity, CSMResourceExtractorData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAUpdateExtractionSpeed() {}

		virtual bool HandleMessage(const WorkerMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceExtractorData& _smData;

		bool _received;
	};
}

#endif // LA_UPDATE_EXTRACTION_SPEED_H_