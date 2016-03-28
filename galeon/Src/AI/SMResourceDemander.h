#ifndef SM_RESOURCE_DEMANDER_H_
#define SM_RESOURCE_DEMANDER_H_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\LAWaitResourceDemand.h"
#include "AI\LAFindProviders.h"
#include "AI\LAReserveFromProviders.h"
#include "AI\LAExecuteResourceDemandTasks.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {
	/**
	Clase con la lógica de SMResourceDemander que gestiona las demandas de recursos de un edificio:
	1) Espera peticiones de demanda de un recurso
	2) Busca proveedores para cubrir la demanda
	3) Reserva todo lo que pueda en cada proveedor
	3) Crea tareas para enviar almas a los proveedores y traer los recursos de vuelta a esta entidad
	*/
	class CSMResourceDemander : public CStateMachine<CLatentAction, CSMResourceDemanderData> {
	public:
		CSMResourceDemander(CEntity* entity) : CStateMachine(entity) {}

		virtual ~CSMResourceDemander() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			int waitRequest = this->addNode(new CLAWaitResourceDemand(entity, _data));
			int findProviders = this->addNode(new CLAFindProviders(entity, _data));
			int reserve = this->addNode(new CLAReserveFromProviders(entity, _data));
			int executeTasks = this->addNode(new CLAExecuteResourceDemandTasks(entity, _data));

			this->addEdge(waitRequest, findProviders, new CConditionSuccess());
			this->addEdge(waitRequest, waitRequest, new CConditionFail());

			this->addEdge(findProviders, reserve, new CConditionSuccess());
			this->addEdge(findProviders, waitRequest, new CConditionFail());

			this->addEdge(reserve, executeTasks, new CConditionSuccess());
			this->addEdge(reserve, waitRequest, new CConditionFail());

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

#endif