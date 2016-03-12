/*
#ifndef LA_TEMPLATE_
#define LA_TEMPLATE_

#include "AI\LatentAction.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	class CLAEmptyTemplate : public CLatentAction {
	public:
		CLAEmptyTemplate(CEntity* entity, CSMEmptyTemplateData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAEmptyTemplate() {}

		virtual bool HandleMessage(const EmptyTemplateMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMEmptyTemplateData _smData;

		bool _requestReceived;
	};
}

#endif
*/