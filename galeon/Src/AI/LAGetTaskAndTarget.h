#ifndef LA_GET_TASK_TARGET_
#define LA_GET_TASK_TARGET_

#include <queue>

#include "LatentAction.h"
#include "Server.h"
#include "SoulTask.h"
#include "WorkSoulTask.h"
#include "BurnSoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\BuildingManager.h"
#include "Logic\SoulsTrialManager.h"

namespace AI {
	class CLAGetTaskAndTarget : public CLatentAction {
		RTTI_DECL;

	public:
		CLAGetTaskAndTarget(CEntity* entity) : CLatentAction(entity) {
			_workTask = nullptr;
			_burnTask = nullptr;

			_pendingSoulsToWork = TSoulsCategoryQueue();
			_pendingSoulsToBurn = TSoulsCategoryQueue();
		}

		virtual ~CLAGetTaskAndTarget()
		{
			if (_workTask)
				deleteTask(_workTask);

			if (_burnTask)
				deleteTask(_burnTask);
		}

		virtual bool HandleMessage(const HellQuartersMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		CSoulTask* _workTask;
		CSoulTask* _burnTask;

		bool sendSoul(bool workTask);

		bool createWorkTask();
		bool createBurnTask();

		void deleteTask(CSoulTask *task)
		{
			delete task;
			task = nullptr;
		}

		/**
		(Categoría de las) Almas restantes por crear y enviar.
		*/
		typedef std::queue<SoulsTrialManager::SoulsCategory> TSoulsCategoryQueue;

		TSoulsCategoryQueue _pendingSoulsToWork;
		TSoulsCategoryQueue _pendingSoulsToBurn;
	};

}

#endif