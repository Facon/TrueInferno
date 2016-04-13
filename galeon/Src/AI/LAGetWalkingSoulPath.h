#ifndef LA_GET_WALKING_SOUL_PATH_H_
#define LA_GET_WALKING_SOUL_PATH_H_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLAGetWalkingSoulPath : public CLatentAction {
		RTTI_DECL;

	public:
		CLAGetWalkingSoulPath(CEntity* entity, CSMSoulData& smData) : CLatentAction(entity), _smData(smData), _pathMessageReceived(false) {}

		virtual ~CLAGetWalkingSoulPath() {}

		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMSoulData& _smData;

		// Flag que refleja si hemos recibido respuesta de mensaje de ruta (aunque la ruta puede ser nula)
		bool _pathMessageReceived;
	};
}

#endif // LA_GET_WALKING_SOUL_PATH_H_