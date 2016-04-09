#ifndef SM_POWER_GENERATOR_H_
#define SM_POWER_GENERATOR_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAWaitConsumerChange.h"
#include "AI\LACheckNewConsumption.h"
#include "AI\LAAnswerConsumer.h"
#include "AI\LAFillReserves.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	/**
	Máquina de estados para los generadores de energía PowerGenerator.
	- Inicialmente se pone a la espera de solicitudes de cambio de consumidores
	- Si llega una solicitud de nuevo consumidor se pasa al estado de comprobación de nuevo consumo
	- Si puede soportar el consumo nuevo:
		- Pasa a responder al consumidor para aceptar la conexión
		- Si no, intenta asegurarse reservas
	- Tras conseguir (o no) reservas pasa al estado de respuesta al consumidor
	- Tras la respuesta al consumidor siempre vuelve al estado de espera
	*/
	class CSMPowerGenerator : public CStateMachine<CLatentAction, CSMPowerGeneratorData> {

	public:
		CSMPowerGenerator(CEntity* entity) : CStateMachine(entity) {
			int waitConsumer = this->addNode(new CLAWaitConsumerChange(entity, _data));
			int checkNewConsumption = this->addNode(new CLACheckNewConsumption(entity, _data));
			int answerConsumer = this->addNode(new CLAAnswerConsumer(entity, _data));
			int fillReserves = this->addNode(new CLAFillReserves(entity, _data));

			// Inicialmente se pone a la espera de solicitudes de cambio de consumidores
			this->setInitialNode(waitConsumer);

			// Si llega una solicitud de nuevo consumidor se pasa al estado de comprobación de nuevo consumo
			this->addEdge(waitConsumer, checkNewConsumption, new CConditionSuccess());
			this->addEdge(waitConsumer, waitConsumer, new CConditionFail());

			/* 
			* Si puede soportar el consumo nuevo :
			*	- Pasa a responder al consumidor para aceptar la conexión
			*	- Si no, intenta asegurarse reservas
			*/
			this->addEdge(checkNewConsumption, answerConsumer, new CConditionSuccess());
			this->addEdge(checkNewConsumption, fillReserves, new CConditionFail());

			// Tras conseguir (o no) reservas pasa al estado de respuesta al consumidor
			this->addEdge(fillReserves, answerConsumer, new CConditionFinished());

			// Tras la respuesta al consumidor siempre vuelve al estado de espera
			this->addEdge(answerConsumer, waitConsumer, new CConditionFinished());

			this->resetExecution();
		}

		virtual ~CSMPowerGenerator() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);

		SM_HANDLE_MESSAGE(ResourceMessage);

		SM_HANDLE_MESSAGE(ConsumptionMessage);

	};
}

#endif // SM_POWER_GENERATOR_H_