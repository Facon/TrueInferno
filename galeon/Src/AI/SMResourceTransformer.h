#ifndef SM_RESOURCE_TRANSFORMER_
#define SM_RESOURCE_TRANSFORMER_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceTransformerData.h"
#include "AI\LAGatherSouls.h"
#include "AI\LABurnSouls.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la lógica de transformación de recursos.

	Cíclicamente se transforman recursos del tipo de entrada en el tipo de salida.
	*/ 
	class CSMResourceTransformer : public CStateMachine<CLatentAction, CSMResourceTransformerData> {
	public:
		CSMResourceTransformer(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceTransformerData>(entity) {}

		virtual ~CSMResourceTransformer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			assert(entityInfo->hasAttribute("transformsFrom"));
			_resourceFrom = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformsFrom"));

			assert(entityInfo->hasAttribute("transformsInto"));
			_resourceInto = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformsInto"));
			
			assert(entityInfo->hasAttribute("transformRatio"));
			_transformRatio = entityInfo->getFloatAttribute("transformRatio");

			assert(entityInfo->hasAttribute("transformPeriod"));
			_period = 1000 * entityInfo->getIntAttribute("transformPeriod");

			assert(entityInfo->hasAttribute("transformCostResource"));
			_costResource = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("transformCostResource"));

			assert(entityInfo->hasAttribute("transformCostRatio"));
			_costRatio = entityInfo->getFloatAttribute("transformCostRatio");

			// TODO
			// Creación de SM en base a los datos
			//int gatherResources = this->addNode(new CLAGatherSouls(_entity, _data, _period, _maxSoulsPerCycle));
			//int transformResources = this->addNode(new CLABurnSouls(_entity, _data, _cokePerSoul, _crudePerSoul));

			//this->addEdge(gatherResources, transformResources, new CConditionFinished());
			//this->addEdge(transformResources, gatherResources, new CConditionFinished());

			//this->setInitialNode(gatherResources);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(NumberMessage);

	private:
		/** Recurso de entrada */
		ResourceType _resourceFrom;

		/** Recurso de salida */
		ResourceType _resourceInto;

		/** Ratio de conversión del recurso de entrada en el recurso de salida */
		float _transformRatio;

		/** Periodo (ms) con que se hace la transformación */
		int _period;

		/** Tipo de recurso para los costes */
		ResourceType _costResource;

		/** Ratio del recurso de costes sobre los recursos de entrada */
		float _costRatio;
	};
}

#endif