#ifndef TRANSPORT_SOUL_TASK_
#define TRANSPORT_SOUL_TASK_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI{

	class CTransportTask : public CSoulTask {

	public:
		CTransportTask(const Logic::TEntityID& targetId) : _targetId(targetId) {};

		virtual ~CTransportTask() {};

		virtual CSoulTask* clone(){
			return new CTransportTask(_targetId);
		}

		bool execute(CMap *map) {
			// Notificamos al objetivo
			CEntity *entity = map->getEntityByID(_targetId);
			ResourceMessage m(MessageType::XXX, 1);
			return m.Dispatch(*_target->getEntity());
		};

	private:
		TEntityID _targetId;

	};

}

#endif