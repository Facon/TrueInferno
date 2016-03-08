#ifndef SM_RESOURCE_EXTRACTOR_
#define SM_RESOURCE_EXTRACTOR_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceExtractorData.h"
#include "AI\LAExtractResources.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la lógica fundamental de los extractores de recursos.

	Los edificios extractores cíclicamente producen recursos.
	*/ 
	class CSMResourceExtractor : public CStateMachine<CLatentAction, CSMResourceExtractorData> {
	public:
		CSMResourceExtractor(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceExtractorData>(entity) {}

		virtual ~CSMResourceExtractor() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			assert(entityInfo->hasAttribute("extractedResource"));
			_extractedResource = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("extractedResource"));

			assert(entityInfo->hasAttribute("maxExtractedQuantity"));
			_maxExtractedQuantity = entityInfo->getIntAttribute("maxExtractedQuantity");

			assert(entityInfo->hasAttribute("extractionPeriod"));
			_extractionPeriod = 1000 * entityInfo->getIntAttribute("extractionPeriod");

			// Creación de SM en base a los datos
			int waitCycle = this->addNode(new CLAWait(_extractionPeriod));
			int extractResources = this->addNode(new CLAExtractResources(_entity, _data, _extractedResource, _maxExtractedQuantity));

			this->addEdge(waitCycle, extractResources, new CConditionSuccess());
			this->addEdge(extractResources, waitCycle, new CConditionSuccess());

			this->setInitialNode(waitCycle);
			this->resetExecution();

			return true;
		}

	private:
		/** Tipo del recurso que se extrae */
		ResourceType _extractedResource;

		/** Cantidad máxima de recurso que da el edificio por ciclo a plena producción */
		unsigned int _maxExtractedQuantity;

		/** Periodo (ms) con que se extraen recursos */
		int _extractionPeriod;
	};
}

#endif