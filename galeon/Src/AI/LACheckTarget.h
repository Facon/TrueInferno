#ifndef LA_CHECK_TARGET_H_
#define LA_CHECK_TARGET_H_

#include "AI\LatentAction.h"
#include "AI\SMSoulData.h"

namespace AI {
	class CLACheckTarget : public CLatentAction {
		RTTI_DECL;

	public:
		CLACheckTarget(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData) {}

		virtual ~CLACheckTarget() {}

	protected:
		LAStatus OnStart();

	private:
		CSMSoulData& _smData;
	};
}

#endif // LA_CHECK_TARGET_H_