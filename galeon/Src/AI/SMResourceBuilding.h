#ifndef SM_RESOURCE_BUILDING_
#define SM_RESOURCE_BUILDING_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\LAAttendResourceBuildingRequest.h"
#include "AI\SMResourceBuildingData.h"

namespace AI {

	class CSMResourceBuilding : public CStateMachine<CLatentAction, CSMResourceBuildingData> {
	public:
		CSMResourceBuilding(CEntity* entity) : CStateMachine(entity) {}

		virtual ~CSMResourceBuilding() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

			// Guardamos el mapa de recursos almacenables
			if (entityInfo->hasAttribute("storedResources")){
				std::istringstream ss(entityInfo->getStringAttribute("storedResources"));
				std::string item;

				// Para cada tipo separado por coma
				while (std::getline(ss, item, ',')) {
					if (item.size()>0)
						// Inicializamos la cantidad de recursos almacenados de ese tipo
						_data.getStoredResources()[ResourcesManager::parseResourceType(item)] = 0;
				}
			}
			else{
				assert(false && "No stored resources have been defined");
			}

			// Guardamos el set con los recursos que este componente provee
			if (entityInfo->hasAttribute("providedResources")){
				std::istringstream ss(entityInfo->getStringAttribute("providedResources"));
				std::string item;

				// Para cada tipo separado por coma
				while (std::getline(ss, item, ',')) {
					// Almacenamos el tipo
					_data.getProvidedResources().emplace(ResourcesManager::parseResourceType(item));
				}
			}

			if (entityInfo->hasAttribute("maxResources")){
				_data.setMaxResources(entityInfo->getIntAttribute("maxResources"));
			}
			else{
				std::cout << "No resource limit has been defined" << std::endl;
			}

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAAttendResourceBuildingRequest(entity, _data));
			this->addEdge(process, process, new CConditionFinished());

			this->setInitialNode(process);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(ResourceMessage);
	};

}

#endif // SM_RESOURCE_BUILDING_