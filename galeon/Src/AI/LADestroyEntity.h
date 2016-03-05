#ifndef LA_DESTROY_ENTITY_
#define LA_DESTROY_ENTITY_

#include "Logic\Entity\Message.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"

namespace AI {
	class CLADestroyEntity : public CLatentAction {
	public:
		CLADestroyEntity(CEntity* entity) : CLatentAction(entity) {}

		virtual ~CLADestroyEntity() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun();
	};
}

#endif