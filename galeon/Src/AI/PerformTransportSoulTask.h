#ifndef PERFORM_TRANSPORT_SOUL_TASK_H_
#define PERFORM_TRANSPORT_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\BuildingType.h"
#include "Logic\Entity\IconType.h"

using namespace Logic;

namespace AI{

	class CPerformTransportSoulTask : public CSoulTask {

	public:
		CPerformTransportSoulTask(CMap *map, const TEntityID& toId, ResourceType resourceType, int resourceQuantity,
			Logic::SoulsTrialManager::SoulsCategory category = Logic::SoulsTrialManager::SoulsCategory::UNKNOWN) :
			CSoulTask(map, toId, category), // El objetivo es el punto de final del transporte donde entregaremos los recursos
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CPerformTransportSoulTask() {};

		virtual CSoulTask* clone(){
			return new CPerformTransportSoulTask(_map, _target, _resourceType, _resourceQuantity, _category);
		}

		bool start(){
			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			if (executor != nullptr){
				// Icono de recurso transportado actualmente por la entidad
				IconMessage m(MessageType::ICON, Logic::IconType::resourceType2IconType(_resourceType));
				assert(m.Dispatch(*executor) && "Can't set resource icon");

				// Determinamos el tipo de edificio al que va
				BuildingType buildingType = getTargetBuildingType();

				if (buildingType != BuildingType::NonBuilding){
					// Icono de edificio destino
					IconMessage m2(MessageType::ICON_ADD, Logic::IconType::buildingType2IconType(buildingType));
					assert(m2.Dispatch(*executor) && "Can't set building icon");
				}
			}

			else{
				assert(false && "There is no executor starting the task");
				// Dejamos que siga sin icono
			}

			return true;
		}

		bool execute() {
			// Chequeamos que el objetivo al que acabamos de llegar siga existiendo
			CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Entrengamos los recursos en el objetivo
				ResourceMessage m;
				m.assembleResourcesChange(_resourceType, _resourceQuantity);
				bool ret = m.Dispatch(*targetEntity);

				// En caso correcto los recursos han sido entregados por lo que podemos limpiar
				if (ret){
					_resourceType = ResourceType::NONE;
					_resourceQuantity = 0;
				}

				return ret;
			}

			// Si no
			else{
				std::cout << "Soul's target for PerformTransportSoulTask has disappeared" << std::endl;

				// No perdemos los recursos. Si el alma desaparece, sus recursos se irán con ella y no hace falta notificarlo al ResourcesManager
				// También podremos querer buscarle otro objetivo

				return true;
			}
		};

	private:
		// Tipo de recurso que porta el alma
		ResourceType _resourceType;

		// Cantidad de recursos que porta el alma
		int _resourceQuantity;

	}; // class CPerformTransportSoulTask

} // namespace AI

#endif // PERFORM_TRANSPORT_SOUL_TASK_H_