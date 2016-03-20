#ifndef SM_RESOURCE_PROVIDER_H_
#define SM_RESOURCE_PROVIDER_H_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\LAWaitResourceProvide.h"
#include "AI\LAPayCosts.h"
#include "AI\LAExecuteResourceProvideTasks.h"
#include "AI\SMResourceProviderData.h"

namespace AI {
	/**
	Clase con la lógica de SMResourceProvider que gestiona las peticiones al edificio como proveedor de recursos
	1) Espera peticiones de proveedor de recursos
	2) Notifica el uso el de recursos
	3) Envía un alma a transportar los recursos
	*/
	class CSMResourceProvider : public CStateMachine<CLatentAction, CSMResourceProviderData> {
	public:
		CSMResourceProvider(CEntity* entity) : CStateMachine(entity) {}

		virtual ~CSMResourceProvider() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			int waitRequest = this->addNode(new CLAWaitResourceProvide(entity, _data));
			int payCosts = this->addNode(new CLAPayCosts(entity, _data));
			int executeTasks = this->addNode(new CLAExecuteResourceProvideTasks(entity, _data));

			this->addEdge(waitRequest, executeTasks, new CConditionSuccess());
			this->addEdge(waitRequest, waitRequest, new CConditionFail());



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