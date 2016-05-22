#ifndef WORK_SOUL_TASK_H_
#define WORK_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\Maps\Managers\WorkManager.h"
#include "Logic\Entity\IconType.h"
#include "Logic/Entity/Components/Billboard.h"

namespace AI{

	class CWorkTask : public CSoulTask {

	public:
		CWorkTask(Logic::CMap *map, const Logic::TEntityID& target, Logic::CSoulsTrialManager::SoulsCategory category, bool started = false) :
			CSoulTask(map, target, category), _started(started) {};

		virtual ~CWorkTask() {};

		virtual CSoulTask* clone()
		{
			return new CWorkTask(_map, _target, _category, _started);
		}

		bool start()
		{
			// Evitamos múltiples inicializaciones
			if (_started)
				return true;

			_started = true;

			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si existe
			if (targetEntity != nullptr)
			{
				// Asignamos el trabajador al objetivo
				Logic::WorkerMessage m(MessageType::WORKER_ASSIGNED, 1);

				// Damos por ejecutada la tarea si nos aceptan el mensaje
				return m.Dispatch(*targetEntity);
			}
			// Si no, la solución más sencilla es descartar al trabajador
			// TODO Quedaría mejor si le asignamos otra tarea
			else
			{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true; // TODO Se debe notificar el error de alguna forma porque en este punto ya sabemos que, sin objetivo, la tarea no va a poder completarse
			}

			return true;
		};

		bool execute()
		{
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Activamos el trabajador en el objetivo...
				Logic::WorkerMessage m(MessageType::WORKER_ACTIVATED, 1);

				// ...si se acepta el mensaje
				bool workerActivated = m.Dispatch(*targetEntity);

				if (!workerActivated)
				{
					CWorkBuilding* workBuilding = targetEntity->getComponent<CWorkBuilding>();
					workBuilding->decreaseAssignedWorkers(1);
				}

				return workerActivated;
			}
			// Si no, la solución más sencilla es descartar al trabajador
			// TODO Quedaría mejor si le asignamos otra tarea
			else{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true;
			}
		};

		void setStarted(bool started)
		{
			_started = started;
		}

	protected:
		// Indica si el método start() ha sido ya ejecutado previamente
		bool _started = false;

	};

} // namespace AI

#endif