#ifndef WORK_SOUL_TASK_H_
#define WORK_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\Maps\Managers\WorkManager.h"
#include "Logic\Entity\IconType.h"
#include "Logic\Entity\Components\Billboard.h"

namespace AI
{

	class CWorkTask : public CSoulTask {

	public:
		CWorkTask(Logic::CMap *map, const Logic::TEntityID& target, SoulsTrialManager::SoulsCategory category, bool workerAssigned = false) :
			CSoulTask(map, target, category), _workerAssigned(workerAssigned) {};

		virtual ~CWorkTask() {};

		virtual CSoulTask* clone()
		{
			return new CWorkTask(_map, _target, _category, _workerAssigned);
		}

		bool start()
		{
			CSoulTask::start();

			// Evitamos múltiples asignaciones a edificio
			if (!_workerAssigned)
			{
				// Chequeamos que el objetivo siga existiendo
				Logic::CEntity* targetEntity = _map->getEntityByID(_target);

				// Si existe
				if (targetEntity != nullptr)
				{
					// Incrementamos el número de trabajadores asignados al edificio de destino
					CWorkBuilding *workBuilding = targetEntity->getComponent<CWorkBuilding>();

					if (workBuilding != nullptr)
						workBuilding->increaseAssignedWorkers(1);

					_workerAssigned = true;
				}
				// Si no, la solución más sencilla es descartar al trabajador
				// TODO Quedaría mejor si le asignamos otra tarea
				else
				{
					std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
					return false; // TODO Se debe notificar el error de alguna forma porque en este punto ya sabemos que, sin objetivo, la tarea no va a poder completarse
				}
			}

			// Ponemos los iconos
			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			// Si no, dejamos que siga sin icono
			if (executor != nullptr)
			{
				// Icono de alma que va a trabajar
				IconMessage m(MessageType::ICON_ADD, IconType::IconType::SOUL);
				bool result = m.Dispatch(*executor);
				assert(result && "Can't set working soul icon");

				// Icono por edificio de destino
				result = addDestinationBuildingIcon();
				assert(result && "Can't set building icon");
			}

			return true;
		};

		bool execute()
		{
			CSoulTask::execute();

			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si existe
			if (targetEntity != nullptr)
			{
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
		}

		void setWorkerAssigned(bool workerAssigned)
		{
			_workerAssigned = workerAssigned;
		}

	protected:
		// Indica si el trabajador ya ha sido asignado a un edificio
		bool _workerAssigned = false;

	}; // class CWorkTask

} // namespace AI

#endif