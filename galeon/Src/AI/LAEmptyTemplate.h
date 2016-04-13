/*
#ifndef LA_TEMPLATE_H_
#define LA_TEMPLATE_H_

#include "AI\LatentAction.h"
#include "AI\SMEmptyTemplateData.h"

namespace AI {
	class CLAEmptyTemplate : public CLatentAction {
		RTTI_DECL;

	public:
		CLAEmptyTemplate(CEntity* entity, CSMEmptyTemplateData& smData) : CLatentAction(entity), _smData(smData), _received(false) {}

		virtual ~CLAEmptyTemplate() {}

		virtual bool HandleMessage(const XXXMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMEmptyTemplateData& _smData;

		bool _received;
	};
}

#endif // LA_TEMPLATE_H_
*/