#ifndef SM_POWER_CONSUMER_H_
#define SM_POWER_CONSUMER_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAFindGenerator.h"
#include "AI\LAAttachToGenerator.h"
#include "AI\LAWaitGeneratorDetachment.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	/**
	M�quina de estados para los consumidores de energ�a PowerConsumer.
	- La entidad comienza en el estado de b�squeda de generador
	- Permanece en b�squeda hasta que encuentra uno y pasa a intentar conectarse
	- Si la conexi�n va bien pasa al estado de espera, si no, vuelve a buscar otro generador
	- En estado de espera no se hace nada hasta recibir desconexi�n. Si se desconecta, intenta buscar otro generador
	*/
	class CSMPowerConsumer : public CStateMachine<CLatentAction, CSMPowerConsumerData> {
	public:
		CSMPowerConsumer(CEntity* entity) : CStateMachine(entity) {
			// Empezamos en estado de b�squeda de generador
			int findGenerator = this->addNode(new CLAFindGenerator(entity));
			int attachToGenerator = this->addNode(new CLAAttachToGenerator(entity));
			int waitDetachment = this->addNode(new CLAWaitGeneratorDetachment(entity));

			this->addEdge(findGenerator, attachToGenerator, new CConditionSuccess());
			this->addEdge(findGenerator, findGenerator, new CConditionFail());

			this->addEdge(attachToGenerator, waitDetachment, new CConditionSuccess());
			this->addEdge(attachToGenerator, findGenerator, new CConditionFail());

			this->addEdge(waitDetachment, findGenerator, new CConditionSuccess());
			this->addEdge(waitDetachment, waitDetachment, new CConditionFail());

			this->setInitialNode(findGenerator);
			this->resetExecution();
		}

		virtual ~CSMPowerConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);
	};
}

#endif // SM_POWER_CONSUMER_H_