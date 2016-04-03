#ifndef SM_SOUL_H_SENDER_
#define SM_SOUL_H_SENDER_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\LASendSoul.h"

namespace AI {
	class CSMSoulSenderData {
	public:
		CSMSoulSenderData() {}
		virtual ~CSMSoulSenderData() {}
	};

	class CSMSoulSender : public CStateMachine<CLatentAction, CSMSoulSenderData> {
	public:
		CSMSoulSender(CEntity* entity) : CStateMachine(entity) {
			CSMSoulSenderData data;

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLASendSoul(entity));
			this->addEdge(process, process, new CConditionFinished());
			
			this->setInitialNode(process);
			this->resetExecution();
		}

		virtual ~CSMSoulSender() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(SoulSenderMessage);
	};
}

#endif