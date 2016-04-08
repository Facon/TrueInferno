#ifndef SM_RESOURCE_CONSUMER_H_
#define SM_RESOURCE_CONSUMER_H_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceConsumerData.h"
#include "AI\LAConsumeResources.h"
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
			
			// TODO
			/*
			// Creación de SM en base a los datos
			int stop = this->addNode(new CLAWait());
			int wait = this->addNode(new CLAWait(_consumptionPeriod));
			int reserve = this->addNode(new CLAReserveResourcesToConsume(_entity, _data, _consumedResource));
			int consume = this->addNode(new CLAConsumeResources(_entity, _data, _consumedResource));

			// Inicialmente la entidad empieza en estado parado
			this->setInitialNode(stop);

			// Pasa al estado de espera
			this->addEdge(stop, wait, new CConditionMessage<CLatentAction, ConsumeMessage>()

			this->addEdge(wait, reserve, new CConditionFinished());

			this->addEdge(reserve, consume, new CConditionSuccess());
			this->addEdge(reserve, empty, new CConditionFail());
			*/

			this->resetExecution();

			return true;
		}

		bool SMGlobalHandleMessage(const PowerMessage& msg){
			if (msg._type != MessageType::POWER_ATTACHMENT_INFO)
				return false;

			// Si hay conexión el consumo aumenta, si es desconexión disminuye
			_data.modifyConsumption(msg._attach ? msg._consumption : -msg._consumption);

			std::cout << "Current consumption = " << _data.getConsumption() << std::endl;

			return true;
		}

		SM_HANDLE_MESSAGE_WGLOBAL(PowerMessage);
		
		SM_HANDLE_MESSAGE(ConsumptionMessage);

	private:
		/** Tipo del recurso que se consume */
		ResourceType _consumedResource;

		/** Periodo (ms) con que se consumen recursos */
		int _consumptionPeriod;
	};
}

#endif