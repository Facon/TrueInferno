#ifndef WORK_SOUL_TASK_H_
#define WORK_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CWorkTask : public CSoulTask {

	public:
		CWorkTask(Logic::CMap *map, const Logic::TEntityID& target) : CSoulTask(map, target) {};

		virtual ~CWorkTask() {};

		virtual CSoulTask* clone(){
			return new CWorkTask(_map, _target);
		}

		bool execute() {
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Notificamos al objetivo
				Logic::WorkerMessage m(1);
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	};

}

#endif