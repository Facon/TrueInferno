#ifndef LA_RECOUNT_RESOURCES_H_
#define LA_RECOUNT_RESOURCES_H_

#include "AI\LatentAction.h"
#include "Logic\Entity\Message.h"

namespace AI {
	template <class SharedData>
	class CLARecountResources : public CLatentAction {

	public:
		CLARecountResources(CEntity* entity, SharedData& smData, ResourceType resourceFrom) : CLatentAction(entity), _smData(smData), _resourceType(resourceFrom) {}

		virtual ~CLARecountResources() {}

		RTTI_DECL_TEMPLATE(SharedData);

		bool HandleMessage(const ResourceMessage& msg) {
			// Rechazamos lo que no sean mensajes de información de recursos
			if (msg._type != MessageType::RESOURCES_INFO)
				return false;

			// Rechazamos si no es el tipo de recurso que queríamos saber
			if (msg._resourceType != _resourceType)
				return false;

			// En principio parece viable aceptar peticiones simultáneas. Si da problemas descomentar lo siguiente para evitarlas
			/*if (_requestReceived)
			return false;*/

			_requestReceived = true;

			// Guardamos la info en la memoria compartida
			_smData.setResourceType(msg._resourceType);
			_smData.setMaxResources(msg._max);
			_smData.setStoredResources(msg._stored);
			_smData.setAvailableResources(msg._available);

			// Reactivamos la LA
			resume();

			return true;
		}

	protected:
		CLatentAction::LAStatus OnStart() {
			// Inicializamos
			_requestReceived = false;

			// Solicitamos información de recursos a nosotros mismos
			ResourceMessage m;
			m.assembleResourcesAsk(_resourceType, _entity->getEntityID());

			// Si tenemos respuesta
			if (m.Dispatch(*_entity))
				// Suspenderemos la LA hasta que llegue un mensaje con los datos solicitados
				return LAStatus::SUSPENDED;

			// Si no, seguimos en OnStart
			else
				return LAStatus::READY;
		}

		CLatentAction::LAStatus OnRun(unsigned int msecs) {
			return LAStatus::SUCCESS;
		}

	private:
		SharedData& _smData;
		
		const ResourceType _resourceType;

		bool _requestReceived;
	};

	RTTI_IMPL_TEMPLATE(CLARecountResources, CLatentAction, SharedData)
}
		
#endif // LA_RECOUNT_RESOURCES_H_