/*
#ifndef LA_TEMPLATE_H_
#define LA_TEMPLATE_H_

#include "AI\LatentAction.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	class CLAEmptyTemplate : public CLatentAction {
	public:
		CLAEmptyTemplate(CEntity* entity, CSMEmptyTemplateData& smData) : CLatentAction(entity), _smData(smData), _requestReceived(false) {}

		virtual ~CLAEmptyTemplate() {}

		virtual bool HandleMessage(const XXXMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMEmptyTemplateData& _smData;

		bool _requestReceived;
	};
}

#endif // LA_TEMPLATE_H_
*/