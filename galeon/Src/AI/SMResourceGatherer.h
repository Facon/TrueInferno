#ifndef SM_RESOURCE_GATHERER_H_
#define SM_RESOURCE_GATHERER_H_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMResourceGathererData.h"
#include "AI\LARecountResources.h"
#include "AI\LAAskResources.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

#include <set>

namespace AI {
	/**
	FSM que realiza una búsqueda cíclica de recursos.

	Por cada tipo de recurso almacenado:
	- Cuenta los recursos de partida del recurso
	- En base a la cantidad actual y límite de los recursos, solicita la máxima cantidad posible que no desborde dicha capacidad
	*/
	class CSMResourceGatherer : public CStateMachine<CLatentAction, CSMResourceGathererData> {
	public:
		CSMResourceGatherer(CEntity* entity) : CStateMachine<CLatentAction, CSMResourceGathererData>(entity) {}

		virtual ~CSMResourceGatherer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			// Guardamos el conjunto de recursos que se recogen
			if (entityInfo->hasAttribute("gatheredResources")){
				std::istringstream ss(entityInfo->getStringAttribute("gatheredResources"));
				std::string item;

				// Para cada tipo separado por coma
				while (std::getline(ss, item, ',')) {
					if (item.size()>0){
						ResourceType resourceType = ResourcesManager::parseResourceType(item);
						//_data.addGatheredResource(resourceType);
						_gatheredResources.emplace(resourceType);
					}
				}
			}
			else{
				assert(false && "No gathered resources have been defined");
			}

			assert(entityInfo->hasAttribute("gatheringPeriodSecs"));
			int _period = 1000 * entityInfo->getIntAttribute("gatheringPeriodSecs");

			// Creación de SM en base a los datos

			// Por cada recurso se añade una fase de recuento y de solicitud
			auto it = _gatheredResources.cbegin();

			// El primer recurso lo hacemos aparte
			ResourceType firstResourceType = (*it);

			// Creamos estados y transición
			int firstRecount = this->addNode(new CLARecountResources<CSMResourceGathererData>(_entity, _data, firstResourceType));
			int ask = this->addNode(new CLAAskResources<CSMResourceGathererData>(_entity, _data, firstResourceType));
			this->addEdge(firstRecount, ask, new CConditionFinished());

			// Avanzamos el iterador de recursos
			++it;

			// Establecemos su fase de recuento como la inicial de la FSM
			this->setInitialNode(firstRecount);

			// Para el resto de recursos
			while(it != _gatheredResources.cend()){
				// Leemos el recurso
				ResourceType resourceType = (*it);

				// Guardamos el id de los anteriores estados
				int previousAsk = ask;

				// Creamos los estados
				int recount = this->addNode(new CLARecountResources<CSMResourceGathererData>(_entity, _data, resourceType));
				ask = this->addNode(new CLAAskResources<CSMResourceGathererData>(_entity, _data, resourceType));

				// Añadimos la transición entre la última fase del recurso anterior a la primera de éste
				this->addEdge(previousAsk, recount, new CConditionFinished());

				// Y la transición entre fases del recurso actual
				this->addEdge(recount, ask, new CConditionFinished());

				// Incrementamos iterador
				++it;
			}

			// Creamos el último estado, el de espera
			int wait = this->addNode(new CLAWait(_period));

			// Añadimos la transición entre la última fase del último recurso a la espera
			this->addEdge(ask, wait, new CConditionFinished());

			// Y de la espera a la primera fase del primer recurso para cerrar el bucle
			this->addEdge(wait, firstRecount, new CConditionFinished());
			
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(ResourceMessage);

	private:
		// Conjunto de recursos recogidos por la entidad
		std::set<ResourceType> _gatheredResources;
	};
}

#endif // SM_RESOURCE_GATHERER_H_