#ifndef SM_RESOURCE_PROVIDER_H_
#define SM_RESOURCE_PROVIDER_H_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\LAWaitResourceProvide.h"
#include "AI\LAPickUpResources.h"
#include "AI\LAExecuteResourceProvideTasks.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	/**
	Clase con la lógica de SMResourceProvider que gestiona las peticiones al edificio como proveedor de recursos
	1) Espera peticiones de provisión de recursos
	2) Recoge los recursos reservados
	3) Envía un alma a transportar los recursos de vuelta al que los pidió
	*/
	class CSMResourceProvider : public CStateMachine<CLatentAction, CSMResourceProviderData> {
	public:
		CSMResourceProvider(CEntity* entity) : CStateMachine(entity) {}

		virtual ~CSMResourceProvider() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			int waitRequest = this->addNode(new CLAWaitResourceProvide(entity, _data));
			int pickupResources = this->addNode(new CLAPickUpResources(entity, _data));
			int executeTasks = this->addNode(new CLAExecuteResourceProvideTasks(entity, _data));

			this->addEdge(waitRequest, pickupResources, new CConditionSuccess());
			this->addEdge(waitRequest, waitRequest, new CConditionFail());

			this->addEdge(pickupResources, executeTasks, new CConditionSuccess());
			this->addEdge(pickupResources, waitRequest, new CConditionFail());

			this->addEdge(executeTasks, waitRequest, new CConditionSuccess());
			this->addEdge(executeTasks, waitRequest, new CConditionFail());

			this->setInitialNode(waitRequest);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(LogisticsMessage);
		SM_HANDLE_MESSAGE(ResourceMessage);
	};
}

#endif // SM_RESOURCE_PROVIDER_H_