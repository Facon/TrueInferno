#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(Logic::CMap *map, const Logic::TEntityID& target) : CSoulTask(map, target) {};

		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_map, _target);
		}

		bool start(){
			return true;
		}

		bool execute() {
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Notificamos al objetivo
				NumberMessage m(MessageType::FURNACE_BURN_SOULS, 1);
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