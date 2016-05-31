#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\Entity\IconType.h"

namespace AI{

	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(Logic::CMap *map, const Logic::TEntityID& target, Logic::SoulsTrialManager::SoulsCategory category) :
			CSoulTask(map, target, category) {};

		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_map, _target, _category);
		}

		bool start(){
			CSoulTask::start();

			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			if (executor != nullptr){
				// Icono de alma quemándose
				IconMessage m(MessageType::ICON_ADD, IconType::IconType::BURNING);
				bool result = m.Dispatch(*executor);
				assert(result && "Can't set burning soul icon");

				// Icono por edificio de destino
				result = addDestinationBuildingIcon();
				assert(result && "Can't set building icon");
			}

			else{
				assert(false && "There is no executor starting the task");
				// Dejamos que siga sin icono
			}

			return true;
		}

		bool execute()
		{
			CSoulTask::execute();

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

	}; // class CBurnTask

} // namespace AI

#endif