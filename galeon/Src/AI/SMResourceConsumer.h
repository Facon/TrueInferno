#ifndef SM_RESOURCE_CONSUMER_H_
#define SM_RESOURCE_CONSUMER_H_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceConsumerData.h"
#include "AI\LAReserveResourcesToConsume.h"
#include "AI\LAConsumeResources.h"
#include "AI\LAAcceptConsumptionChanges.h"
#include "AI\LAStopConsumingResources.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la lógica de consumo periódico de recursos de una entidad.
	*/ 
	class CSMResourceConsumer : public CStateMachine<CLatentAction, CSMResourceConsumerData> {
	public:
		CSMResourceConsumer(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceConsumerData>(entity) {}

		virtual ~CSMResourceConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			assert(entityInfo->hasAttribute("consumedResource"));
			_consumedResource = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("consumedResource"));

			assert(entityInfo->hasAttribute("consumptionPeriod"));
			_consumptionPeriod = 1000 * entityInfo->getIntAttribute("consumptionPeriod");
			
			// Creación de SM en base a los datos
			int stopped = this->addNode(new CLAWait());
			int waiting = this->addNode(new CLAWait(_consumptionPeriod));
			int reserving = this->addNode(new CLAReserveResourcesToConsume(_entity, _data, _consumedResource));
			int consuming = this->addNode(new CLAConsumeResources(_entity, _data, _consumedResource));
			int accept = this->addNode(new CLAAcceptConsumptionChanges(_entity, _data, _consumedResource));
			int stopping = this->addNode(new CLAStopConsumingResources(_entity, _data, _consumedResource));

			// Inicialmente la entidad empieza en estado parado
			this->setInitialNode(stopped);

			// Pasa al estado de espera cuando se le solicita empezar a consumir
			this->addEdge(stopped, waiting, new CConditionMessage<CLatentAction, ConsumptionMessage>(TMessage::CONSUMPTION_START));

			// Una vez finalizada la espera, reserva recursos para poder consumirlos
			this->addEdge(waiting, reserving, new CConditionFinished());

			// Si tuvo éxito la reserva, pasa a consumirlos. Si no, falla y empieza a pararse
			this->addEdge(reserving, consuming, new CConditionSuccess());
			this->addEdge(reserving, stopping, new CConditionFail());

			// Después de consumir se aceptan nuevos consumos
			this->addEdge(consuming, accept, new CConditionFinished());

			// Y se pone a la espera del siguiente ciclo de consumo
			this->addEdge(accept, waiting, new CConditionFinished());
			
			// Parada en dos fases
			this->addEdge(stopping, stopped, new CConditionFinished());

			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(ConsumptionMessage);

		SM_HANDLE_MESSAGE(ResourceMessage);

	private:
		/** Tipo del recurso que se consume */
		ResourceType _consumedResource;

		/** Periodo (ms) con que se consumen recursos */
		int _consumptionPeriod;
	};
}

#endif