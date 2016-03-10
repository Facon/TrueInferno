#ifndef SM_LOGISTICS_
#define SM_LOGISTICS_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\LAWaitLogisticsRequest.h"
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
			// TODO
			//int findProviders = this->addNode(new CLAFindLogisticsTargets(entity, _data));
			//int createTasks = this->addNode(new CLACreateLogisticsTasks(entity, _data));
			//int executeTasks = this->addNode(new CLAExecuteLogisticsTasks(entity, _data));

			// TODO
			//this->addEdge(waitRequest, findProviders, new CConditionFinished());
			//this->addEdge(findProviders, createTasks, new CConditionFinished());
			//this->addEdge(createTasks, executeTasks, new CConditionFinished());
			//this->addEdge(executeTasks, waitRequest, new CConditionFinished());

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