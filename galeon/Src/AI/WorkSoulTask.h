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
		CWorkTask(Logic::CMap *map, const Logic::TEntityID& target, Logic::SoulsTrialManager::SoulsCategory category) :
			CSoulTask(map, target, category), _workerAssigned(false) {};

		virtual ~CWorkTask() {};

		virtual CSoulTask* clone(){
			return new CWorkTask(_map, _target, _category);
		}

		bool start(){
			CSoulTask::start();

			if (!_workerAssigned){
				// Chequeamos que el objetivo siga existiendo
				Logic::CEntity* targetEntity = _map->getEntityByID(_target);

				// Si lo está
				if (targetEntity != nullptr){
					// Asignamos el trabajador al objetivo
					Logic::WorkerMessage m(MessageType::WORKER_ASSIGNED, 1);

					// Paramos si no nos aceptan el mensaje
					if (!m.Dispatch(*targetEntity))
						return false;

					_workerAssigned = true;
				}

				// Si no, la solución más sencilla es descartar al trabajador
				// TODO Quedaría mejor si le asignamos otra tarea
				else{
					std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
					return true; // TODO Se debe notificar el error de alguna forma porque en este punto ya sabemos que, sin objetivo, la tarea no va a poder completarse
				}
			}

			// Ponemos los iconos
			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			if (executor != nullptr){
				// Icono de alma que va a trabajar
				IconMessage m(MessageType::ICON_ADD, IconType::IconType::SOUL);
				const bool result = m.Dispatch(*executor);
				assert(result && "Can't set working soul icon");

				// Determinamos el tipo de edificio al que va
				BuildingType buildingType = getTargetBuildingType();

				if (buildingType != BuildingType::NonBuilding && buildingType != BuildingType::Unassigned){
					// Icono de edificio destino
					IconMessage m2(MessageType::ICON_ADD, Billboard::getBuildingIcon(buildingType));
					const bool result = m2.Dispatch(*executor);
					assert(result && "Can't set building icon");
				}
			}

			else{
				assert(false && "There is no executor starting the task");
				// Dejamos que siga sin icono
			}

			return true;
		};

		bool execute() {
			CSoulTask::execute();

			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Activamos el trabajador en el objetivo
				Logic::WorkerMessage m(MessageType::WORKER_ACTIVATED, 1);
				return m.Dispatch(*targetEntity);
			}

			// Si no, la solución más sencilla es descartar al trabajador
			// TODO Quedaría mejor si le asignamos otra tarea
			else{
				std::cout << "Soul's target for WorkSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	private:
		bool _workerAssigned;

	}; // class CWorkTask

} // namespace AI

#endif