#ifndef SM_LOGISTICS_
#define SM_LOGISTICS_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\LAWaitLogisticsRequest.h"
#include "AI\LAFindProviders.h"
#include "AI\LAExecuteLogisticsTasks.h"
#include "AI\Server.h"

namespace AI {
	/**
	Clase con la lógica de SMLogistics que gestiona las tareas de logística de recursos de un edificio
	- En el estado inicial espera peticiones de otras entidades/componentes
	- Si recibe una petición pasa al estado de proceso
	- Se queda a la espera 

	*/
	class CSMLogistics : public CStateMachine<CLatentAction, CSMLogisticsData> {
	public:
		CSMLogistics(CEntity* entity) : CStateMachine(entity) {
			int waitRequest = this->addNode(new CLAWaitLogisticsRequest(entity, _data));
			int findProviders = this->addNode(new CLAFindProviders(entity, _data));
			int executeTasks = this->addNode(new CLAExecuteLogisticsTasks(entity, _data));

			this->addEdge(waitRequest, findProviders, new CConditionSuccess());
			this->addEdge(waitRequest, waitRequest, new CConditionFail());

			this->addEdge(findProviders, executeTasks, new CConditionSuccess());
			this->addEdge(findProviders, waitRequest, new CConditionFail());

			this->addEdge(executeTasks, waitRequest, new CConditionSuccess());
			this->addEdge(executeTasks, waitRequest, new CConditionFail());

			this->setInitialNode(waitRequest);
			this->resetExecution();
		}

		virtual ~CSMLogistics() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(LogisticsMessage);
	};
}

#endif