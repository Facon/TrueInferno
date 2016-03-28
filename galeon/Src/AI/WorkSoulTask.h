#ifndef WORK_SOUL_TASK_H_
#define WORK_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\Maps\Managers\WorkManager.h"

namespace AI{

	class CWorkTask : public CSoulTask {

	public:
		CWorkTask(Logic::CMap *map, const Logic::TEntityID& target) : CSoulTask(map, target) {};

		virtual ~CWorkTask() {};

		virtual CSoulTask* clone(){
			return new CWorkTask(_map, _target);
		}

		bool start(){
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Asignamos el trabajador al objetivo
				Logic::WorkerMessage m(TMessage::WORKER_ASSIGNED, 1);
				return m.Dispatch(*targetEntity);
			}

			// Si no, la solución más sencilla es descartar al trabajador
			// TODO Quedaría mejor si le asignamos otra tarea
			else{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true; // TODO Se debe notificar el error de alguna forma porque en este punto ya sabemos que, sin objetivo, la tarea no va a poder completarse
			}
		};

		bool execute() {
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Activamos el trabajador en el objetivo
				Logic::WorkerMessage m(TMessage::WORKER_ACTIVATED, 1);
				return m.Dispatch(*targetEntity);
			}

			// Si no, la solución más sencilla es descartar al trabajador
			// TODO Quedaría mejor si le asignamos otra tarea
			else{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	};

}

#endif