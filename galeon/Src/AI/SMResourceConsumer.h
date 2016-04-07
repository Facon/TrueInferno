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

			// Creación de SM en base a los datos
			int waitCycle = this->addNode(new CLAWait(_consumptionPeriod));
			int extractResources = this->addNode(new CLAConsumeResources(_entity, _data, _consumedResource));

			this->addEdge(waitCycle, extractResources, new CConditionFinished());
			this->addEdge(extractResources, waitCycle, new CConditionFinished());

			this->setInitialNode(waitCycle);
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

	private:
		/** Tipo del recurso que se consume */
		ResourceType _consumedResource;

		/** Periodo (ms) con que se consumen recursos */
		int _consumptionPeriod;
	};
}

#endif