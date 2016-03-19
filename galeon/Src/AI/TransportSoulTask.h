#ifndef TRANSPORT_SOUL_TASK_
#define TRANSPORT_SOUL_TASK_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CTransportTask : public CSoulTask {

	public:
		CTransportTask(Logic::CMap *map, const Logic::TEntityID& fromId, const Logic::TEntityID& toId, ResourceType resourceType, unsigned int resourceQuantity) :
			CSoulTask(map, fromId),
			_fromId(fromId), 
			_toId(toId),
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CTransportTask() {};

		virtual CSoulTask* clone(){
			return new CTransportTask(_map, _fromId, _toId, _resourceType, _resourceQuantity);
		}

		bool execute() {
			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo est�
			if (targetEntity != nullptr){
				// Notificamos al objetivo
				LogisticsMessage m(LogisticsAction::BRING_RESOURCES_TO, _resourceType, _resourceQuantity, _toId);
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for BurnSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	private:
		// Entidad desde la que debe partir el alma para comenzar el transporte
		TEntityID _fromId;

		// Entidad hasta la que deber� ir finalmente el alma
		TEntityID _toId;

		// Tipo de recurso que porta el alma
		ResourceType _resourceType;

		// Cantidad de recursos que porta el alma
		unsigned int _resourceQuantity;
	};

}

#endif