#ifndef LA_ASK_RESOURCES_H_
#define LA_ASK_RESOURCES_H_

#include "AI\LatentAction.h"
#include "Logic\Entity\Message.h"

namespace AI {
	/**
	* Representa una acción de solicitar recursos.
	* @param SharedData objeto de datos responsable de proporcionar la cantidad de recursos a solicitar
	*/
	template <class SharedData>
	class CLAAskResources : public CLatentAction {

	public:
		CLAAskResources(CEntity* entity, SharedData& smData, ResourceType resourceType) :
			CLatentAction(entity), _smData(smData), _resourceType(resourceType) {}

		virtual ~CLAAskResources() {}

		RTTI_DECL_TEMPLATE(SharedData);

	protected:

		CLatentAction::LAStatus OnStart() {
			return LAStatus::RUNNING;
		}

		CLatentAction::LAStatus OnRun(unsigned int msecs) {
			// Solicitamos al objeto de datos la cantidad de recursos a solicitar
			int needed = _smData.getResourcesToAsk(_resourceType);

			// Si la cuenta sale negativa ó 0 es que ya estamos a tope y no necesitamos nada
			if (needed <= 0){
				needed = 0;
				return LAStatus::SUCCESS;
			}

			// Pedimos los recursos calculados
			LogisticsMessage m;
			m.assembleDemandResources(_resourceType, needed);

			// Intentamos transmitir la petición
			if (m.Dispatch(*_entity))
				// Y acabamos
				return LAStatus::SUCCESS;
			else
				// O volveremos a intentarlo en el siguiente tick
				return LAStatus::RUNNING;
		}

	private:
		SharedData& _smData;

		/** Tipo de recurso a pedir */
		const ResourceType _resourceType;
	};

	RTTI_IMPL_TEMPLATE(CLAAskResources, CLatentAction, SharedData)
}

#endif // LA_ASK_RESOURCES_H_