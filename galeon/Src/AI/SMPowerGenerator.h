#ifndef SM_POWER_GENERATOR_H_
#define SM_POWER_GENERATOR_H_

#include "Logic\Entity\Message.h"

#include "Logic\Maps\Map.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAWaitConsumerChange.h"
#include "AI\LACheckNewConsumption.h"
#include "AI\LAAcceptOrRejectNewConsumer.h"
#include "AI\LAFillReserves.h"
#include "AI\LADetachConsumers.h"
#include "AI\SMPowerGeneratorData.h"

namespace AI {
	/**
	Máquina de estados para los generadores de energía PowerGenerator.
	- Inicialmente se pone a la espera de solicitudes de cambio de consumidores
	- Si llega una solicitud de nuevo consumidor se pasa al estado de comprobación de nuevo consumo
	- Tras comprobar el consumo responde al consumidor
	- Tras la respuesta al consumidor siempre vuelve al estado de espera
	*/
	class CSMPowerGenerator : public CStateMachine<CLatentAction, CSMPowerGeneratorData> {

	public:
		CSMPowerGenerator(CEntity* entity) : CStateMachine(entity) {
			_name = "PowerGenerator";
			_debug = false;

			int waitConsumer = this->addNode(new CLAWaitConsumerChange(entity, _data));
			int checkNewConsumption = this->addNode(new CLACheckNewConsumption(entity, _data));
			int acceptOrReject = this->addNode(new CLAAcceptOrRejectNewConsumer(entity, _data));
			int detachConsumers = this->addNode(new CLADetachConsumers(entity, _data));

			// Inicialmente se pone a la espera de solicitudes de cambio de consumidores
			this->setInitialNode(waitConsumer);

			// Si llega una solicitud de nuevo consumidor se pasa al estado de comprobación de nuevo consumo
			this->addEdge(waitConsumer, checkNewConsumption, new CConditionSuccess());
			this->addEdge(waitConsumer, waitConsumer, new CConditionFail());

			//  Tras comprobar el consumo responde al consumidor
			this->addEdge(checkNewConsumption, acceptOrReject, new CConditionSuccess());
			this->addEdge(checkNewConsumption, acceptOrReject, new CConditionFail());

			// Tras la respuesta al consumidor siempre vuelve al estado de espera
			this->addEdge(acceptOrReject, waitConsumer, new CConditionFinished());

			// Desde cualquier estado, si se recibe un aviso de parada de consumo, hay que pasar a desconectar a todos los clientes
			this->addEdge(waitConsumer, detachConsumers, new CConditionMessage<CLatentAction, ConsumptionMessage>(MessageType::CONSUMPTION_STOPPED));
			this->addEdge(checkNewConsumption, detachConsumers, new CConditionMessage<CLatentAction, ConsumptionMessage>(MessageType::CONSUMPTION_STOPPED));
			this->addEdge(acceptOrReject, detachConsumers, new CConditionMessage<CLatentAction, ConsumptionMessage>(MessageType::CONSUMPTION_STOPPED));

			// Del estado de desconexión se vuelve al de espera inicial
			this->addEdge(detachConsumers, waitConsumer, new CConditionFinished());

			this->resetExecution();
		}

		virtual ~CSMPowerGenerator() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);

		SM_HANDLE_MESSAGE(ResourceMessage);

		SM_HANDLE_MESSAGE(ConsumptionMessage);

		/*
		Desactivación del PowerGenerator. La lógica es prácticamente igual que CLADetachConsumers sólo que aquí es síncrona porque no podemos esperar a otro tick.
		Si da problemas convendría delegar la tarea en el PowerManager
		*/
		void deactivate(){
			// Preparamos el mensaje de desconexión
			PowerMessage powerMsg;
			powerMsg.assemblePowerAttachmentInfo(_entity->getEntityID(), false, 0, -1);

			// Mientras queden consumidores conectados
			auto it = _data.getConsumers().begin();
			while (it != _data.getConsumers().end()){
				TEntityID consumerId = it->first;

				// Localizamos la entidad
				CEntity* consumer = _entity->getMap()->getEntityByID(consumerId);

				// Si no es nula, le intentamos enviar el mensaje
				if (consumer != nullptr){
					assert(powerMsg.Dispatch(*consumer) && "Can't detach existing consumer while deactivating PowerGenerator");
				}

				++it;
			}

			// Borrado de los consumidores
			_data.getConsumers().clear();

			assert(_data.getConsumers().size() == 0 && "Some consumers were not detached");

			// Si había nuevo consumidor esperando respuesta, lo liberamos también
			if (_data.getNewConsumer() != EntityID::UNASSIGNED){
				CEntity *consumer = _entity->getMap()->getEntityByID(_data.getNewConsumer());
				if (consumer != nullptr){
					assert(powerMsg.Dispatch(*consumer) && "Can't detach new consumer while deactivating PowerGenerator");
					_data.setNewConsumer(EntityID::UNASSIGNED);
				}
			}
		}
	};
}

#endif // SM_POWER_GENERATOR_H_