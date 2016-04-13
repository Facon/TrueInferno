#ifndef LA_ASK_WAIT_RESOURCES_
#define LA_ASK_WAIT_RESOURCES_

#include "Logic\Entity\Message.h"
#include "AI\LatentAction.h"
#include "AI\SMResourceTransformerData.h"

namespace AI {
	/**
	Representa la acción de acumular recursos de entrada durante un tiempo determinado
	*/
	class CLAAskAndWaitResources : public CLatentAction {
		RTTI_DECL;

	public:
		CLAAskAndWaitResources(CEntity* entity, CSMResourceTransformerData& smData, ResourceType resourceFrom, unsigned int period) :
			CLatentAction(entity), _smData(smData), _resourceFrom(resourceFrom), _period(period) {}

		virtual ~CLAAskAndWaitResources() {}

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSMResourceTransformerData& _smData;

		/** Tiempo transcurrido desde el último ciclo */
		unsigned int _elapsedTime;

		/** Recurso de entrada */
		const ResourceType _resourceFrom;

		/** Periodo (ms) con que se transforman cíclicamente los recursos */
		const unsigned int _period;
	};

}

#endif