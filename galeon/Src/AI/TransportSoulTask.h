#ifndef TRANSPORT_SOUL_TASK_
#define TRANSPORT_SOUL_TASK_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CTransportTask : public CSoulTask {

	public:
		CTransportTask(const Logic::TEntityID& fromId, const Logic::TEntityID& toId, Logic::ResourceType resourceType, unsigned int resourceQuantity) : 
			_fromId(fromId), 
			_toId(toId),
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CTransportTask() {};

		virtual CSoulTask* clone(){
			return new CTransportTask(_fromId, _toId, _resourceType, _resourceQuantity);
		}

		bool execute(CMap *map) {
			// Notificamos al objetivo
			CEntity *entity = map->getEntityByID(_fromId);
			LogisticsMessage m(LogisticsAction::BRING_RESOURCES_TO, _resourceType, _resourceQuantity, _toId);
			return m.Dispatch(*_target->getEntity());
		};

	private:
		TEntityID _fromId;
		TEntityID _toId;
		Logic::ResourceType _resourceType;
		unsigned int _resourceQuantity;
	};

}

#endif