#ifndef SM_RESOURCE_EXTRACTOR_
#define SM_RESOURCE_EXTRACTOR_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceExtractorData.h"
#include "AI\LAExtractResources.h"
#include "AI\LAUpdateExtractionSpeed.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la l�gica fundamental de los extractores de recursos.

	Los edificios extractores c�clicamente producen recursos en funci�n del n�mero de trabajadores.
	*/ 
	class CSMResourceExtractor : public CStateMachine<CLatentAction, CSMResourceExtractorData> {
	public:
		CSMResourceExtractor(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceExtractorData>(entity) {}

		virtual ~CSMResourceExtractor() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			if(entityInfo->hasAttribute("extractedResource"))
				_extractedResource = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("extractedResource"));
			else {
				assert(false && "extractedResource not defined");
				return false;
			}

			if (entityInfo->hasAttribute("maxExtractedQuantity"))
				_maxExtractedQuantity = entityInfo->getIntAttribute("maxExtractedQuantity");
			else{
				assert(false && "maxExtractedQuantity not defined");
				return false;
			}

			if(entityInfo->hasAttribute("extractPeriodSecs"))
				_extractionPeriod = 1000 * entityInfo->getIntAttribute("extractPeriodSecs");
			else{
				assert(false && "extractPeriodSecs not defined");
				return false;
			}

			if (entityInfo->hasAttribute("extractParticlesDurationMsecs"))
				_extractParticlesDuration = entityInfo->getIntAttribute("extractParticlesDurationMsecs");
			else{
				assert(false && "extractParticlesDurationMsecs not defined");
				return false;
			}

			// Creaci�n de SM en base a los datos
			int waitCycle = this->addNode(new CLAWait(_extractionPeriod));
			int updateExtractionSpeed = this->addNode(new CLAUpdateExtractionSpeed(_entity, _data));
			int extractResources = this->addNode(new CLAExtractResources(_entity, _data, _extractedResource, _maxExtractedQuantity, _extractParticlesDuration));

			this->addEdge(waitCycle, updateExtractionSpeed, new CConditionFinished());
			this->addEdge(updateExtractionSpeed, extractResources, new CConditionFinished());
			this->addEdge(extractResources, waitCycle, new CConditionFinished());

			this->setInitialNode(waitCycle);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(WorkerMessage);

	private:
		/** Tipo del recurso que se extrae */
		ResourceType _extractedResource;

		/** Cantidad m�xima de recurso que da el edificio por ciclo a plena producci�n */
		unsigned int _maxExtractedQuantity;

		/** Periodo (ms) con que se extraen recursos */
		int _extractionPeriod;

		/** Duraci�n (ms) de las part�culas de extracci�n de recursos */
		int _extractParticlesDuration;
	};
}

#endif