#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(Logic::CMap *map, const Logic::TEntityID& target, Logic::SoulsTrialManager::SoulsCategory category) :
			CSoulTask(map, target, category) {};

		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_map, _target, _category);
		}

		bool start()
		{
			return true;
		}

		bool execute()
		{
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr)
			{
				// Notificamos al objetivo
				SoulBurnMessage m(1, _category);
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for BurnSoulTask has disappeared" << std::endl;
				return true;
			}
		};
	};

}

#endif