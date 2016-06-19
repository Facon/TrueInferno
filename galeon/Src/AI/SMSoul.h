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
#include "AI\LABurnEntity.h"
#include "AI\LADestroyEntity.h"
#include "AI\LACheckTarget.h"
#include "AI\SMSoulData.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

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
		CSMSoul(CEntity* entity) : CStateMachine<CLatentAction, CSMSoulData>(entity), _timeBeforeGetNewPath(2000), _timeBeforeDestruction(2000) {}

		virtual ~CSMSoul() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			if (entityInfo->hasAttribute("timeBeforeGetNewPath"))
				_timeBeforeGetNewPath = 1000 * entityInfo->getIntAttribute("timeBeforeGetNewPath");

			if (entityInfo->hasAttribute("timeBeforeDestruction"))
				_timeBeforeDestruction = 1000 * entityInfo->getIntAttribute("timeBeforeDestruction");

			int waitTask = this->addNode(new CLAWaitSoulTask(entity, _data));
			int getPath = this->addNode(new CLAGetWalkingSoulPath(entity, _data));
			int startTask = this->addNode(new CLAStartSoulTask(entity, _data));
			int followPath = this->addNode(new CLAFollowPath(entity, _data));
			int executeTask = this->addNode(new CLAExecuteSoulTask(entity, _data));
			int checkTargetBeforeExecute = this->addNode(new CLACheckTarget(entity, _data));
			int startBurning = this->addNode(new CLABurnEntity(entity, _data));
			int burning = this->addNode(new CLAWait(_timeBeforeDestruction));
			int destroy = this->addNode(new CLADestroyEntity(entity, _data));
			int checkTarget = this->addNode(new CLACheckTarget(entity, _data));
			int waitRetryPath = this->addNode(new CLAWait(_timeBeforeGetNewPath));
			//int getNewTarget = this->addNode(new CLAGetSoulTaskTarget(entity, _data));

			// Inicialmente el alma se pone a la espera de que le asignen tarea
			this->setInitialNode(waitTask);

			// En cuanto tiene tarea, obtiene ruta hasta el objetivo de la tarea
			this->addEdge(waitTask, getPath, new CConditionFinished());

			// Si obtiene ruta comienza la tarea del alma, si no, chequea la validez del objetivo
			this->addEdge(getPath, startTask, new CConditionSuccess());
			this->addEdge(getPath, checkTarget, new CConditionFail());

			// Si empieza correctamente la tarea pasa a seguir la ruta
			this->addEdge(startTask, followPath, new CConditionSuccess());
			this->addEdge(startTask, startBurning, new CConditionFail());

			// Tras acabar la ruta ejecuta la tarea que tenía asignada
			this->addEdge(followPath, checkTargetBeforeExecute, new CConditionMessage<CLatentAction, WalkSoulPathMessage>(MessageType::WALK_SOUL_PATH_FINISHED));
			this->addEdge(followPath, startBurning, new CConditionFail());

			// Chequeo previo a ejecutar (necesario porque executeTask actualmente no puede fallar)
			this->addEdge(checkTargetBeforeExecute, executeTask, new CConditionSuccess());
			this->addEdge(checkTargetBeforeExecute, startBurning, new CConditionFail());

			// Tras ejecutar la tarea se destruye directamente el alma, o se quema si falló
			this->addEdge(executeTask, destroy, new CConditionSuccess());
			this->addEdge(executeTask, startBurning, new CConditionFail());

			// Tras chequear el objetivo, si es válido, espera para reintentar ruta. Si no, se autodestruye
			this->addEdge(checkTarget, waitRetryPath, new CConditionSuccess());
			this->addEdge(checkTarget, startBurning, new CConditionFail());

			this->addEdge(waitRetryPath, getPath, new CConditionFinished());

			// Ante cualquier fallo: comenzamos a quemar -> quemamos -> destruímos
			this->addEdge(startBurning, burning, new CConditionFinished());
			this->addEdge(burning, destroy, new CConditionFinished());

			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(SoulMessage);

		SM_HANDLE_MESSAGE(WalkSoulPathMessage);

	private:
		CSMSoulData _data;

		/** Tiempo (ms) que se deja transcurrir hasta calcular una nueva ruta */
		unsigned int _timeBeforeGetNewPath;

		// Tiempo (ms) que se deja transcurrir hasta que se destruye el alma
		unsigned int _timeBeforeDestruction;
	};
}

#endif // SM_SOUL_H_