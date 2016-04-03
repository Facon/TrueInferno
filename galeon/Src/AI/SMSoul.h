#ifndef SM_SOUL_H_
#define SM_SOUL_H_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAWaitSoulTask.h"
#include "AI\LAGetWalkingSoulPath.h"
#include "AI\LAStartSoulTask.h"
#include "AI\LAFollowPath.h"
#include "AI\LAExecuteSoulTask.h"
#include "AI\LADestroyEntity.h"
#include "AI\LAGetSoulTaskTarget.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"

namespace AI {
	/**
	Esta FSM controla la lógica fundamental de las almas.

	Inicialmente el alma se pone a la espera de que le asignen un objetivo. En cuanto lo tiene solicita cálculo de ruta.
	Cuando le llega la ruta la sigue.
	Tras acabar la ruta ejecuta la tarea que tenía asignada.
	Por último, se autodestruye.
	*/ 
	class CSMSoul : public CStateMachine<CLatentAction, CSMSoulData> {
	public:
		CSMSoul(CEntity* entity) : CStateMachine<CLatentAction, CSMSoulData>(entity) {
			int waitTask = this->addNode(new CLAWaitSoulTask(entity, _data));
			int getPath = this->addNode(new CLAGetWalkingSoulPath(entity, _data));
			int startTask = this->addNode(new CLAStartSoulTask(entity, _data));
			int followPath = this->addNode(new CLAFollowPath(entity, _data));
			int executeTask = this->addNode(new CLAExecuteSoulTask(entity, _data));
			int destroyEntity = this->addNode(new CLADestroyEntity(entity, _data));
			int getNewTarget = this->addNode(new CLAGetSoulTaskTarget(entity, _data));

			// Inicialmente el alma se pone a la espera de que le asignen tarea
			this->setInitialNode(waitTask);

			// En cuanto tiene tarea, obtiene ruta hasta el objetivo de la tarea
			this->addEdge(waitTask, getPath, new CConditionFinished());

			// Si obtiene ruta comienza la tarea del alma, si no, busca otro objetivo
			this->addEdge(getPath, startTask, new CConditionSuccess());
			this->addEdge(getPath, getNewTarget, new CConditionFail());

			// Si empieza correctamente la tarea pasa a seguir la ruta, si no, busca otro objetivo
			this->addEdge(startTask, followPath, new CConditionSuccess());
			this->addEdge(startTask, getNewTarget, new CConditionFail());

			// Tras acabar la ruta ejecuta la tarea que tenía asignada
			this->addEdge(followPath, executeTask, new CConditionMessage<CLatentAction,WalkSoulPathMessage>(MessageType::WALK_SOUL_PATH_FINISHED));
			this->addEdge(followPath, getNewTarget, new CConditionFail());

			// Tras ejecutar la tarea se destruye el alma, si no, busca otro objetivo
			this->addEdge(executeTask, destroyEntity, new CConditionSuccess());
			this->addEdge(executeTask, getNewTarget, new CConditionFail());

			// Cuando tiene que pedir nuevo objetivo, si lo consigue obtener pasa a buscar ruta, si no, se destruye
			this->addEdge(getNewTarget, getPath, new CConditionSuccess());
			this->addEdge(getNewTarget, destroyEntity, new CConditionFail());

			this->resetExecution();
		}

		virtual ~CSMSoul() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(SoulMessage);

		SM_HANDLE_MESSAGE(WalkSoulPathMessage);

	private:
		CSMSoulData _data;
	};
}

#endif // SM_SOUL_H_