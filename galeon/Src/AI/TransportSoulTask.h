#ifndef TRANSPORT_SOUL_TASK_
#define TRANSPORT_SOUL_TASK_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CTransportTask : public CSoulTask {

	public:
		CTransportTask(Logic::CMap *map, const Logic::TEntityID& fromId, const Logic::TEntityID& toId, ResourceType resourceType, unsigned int resourceQuantity) :
			CSoulTask(map, fromId), // El objetivo es el punto de inicio del transporte
			_toId(toId),
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CTransportTask() {};

		virtual CSoulTask* clone(){
			return new CTransportTask(_map, _target, _toId, _resourceType, _resourceQuantity);
		}

		bool execute() {
			// Chequeamos que el objetivo al que acabamos de llegar siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Notificamos al objetivo para que lleve los recursos al destino
				LogisticsMessage m(LogisticsAction::BRING_RESOURCES_TO, _resourceType, _resourceQuantity, _toId);
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for TransportTask has disappeared" << std::endl;
				return true;
			}
		};

	private:
		// Entidad desde la que debe partir el alma para comenzar el transporte. Es el objetivo inicial, _target, hacia el que debe ir el alma
		//TEntityID _fromId;

		// Entidad hasta la que deberá ir finalmente el alma
		TEntityID _toId;

		// Tipo de recurso que porta el alma
		ResourceType _resourceType;

		// Cantidad de recursos que porta el alma
		unsigned int _resourceQuantity;
	};

}

#endif