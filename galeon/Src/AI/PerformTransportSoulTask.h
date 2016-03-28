#ifndef PERFORM_TRANSPORT_SOUL_TASK_H_
#define PERFORM_TRANSPORT_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"

using namespace Logic;

namespace AI{

	class CPerformTransportSoulTask : public CSoulTask {

	public:
		CPerformTransportSoulTask(CMap *map, const TEntityID& toId, ResourceType resourceType, unsigned int resourceQuantity) :
			CSoulTask(map, toId), // El objetivo es el punto de final del transporte donde entregaremos los recursos
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CPerformTransportSoulTask() {};

		virtual CSoulTask* clone(){
			return new CPerformTransportSoulTask(_map, _target, _resourceType, _resourceQuantity);
		}

		bool start(){
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
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for PerformTransportSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	private:
		// Tipo de recurso que porta el alma
		ResourceType _resourceType;

		// Cantidad de recursos que porta el alma
		unsigned int _resourceQuantity;
	};

}

#endif // PERFORM_TRANSPORT_SOUL_TASK_H_