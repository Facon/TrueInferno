#include "ResourceBuilding.h"
#include "Application/GaleonApplication.h"
#include "Application/GameState.h"

#include <string>
#include <iostream>
#include <sstream>

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceBuilding);
	IMP_FACTORY(CResourceBuilding);

	bool CResourceBuilding::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		// Guardamos el mapa de recursos almacenables
		if (entityInfo->hasAttribute("storedResources")){
			std::istringstream ss(entityInfo->getStringAttribute("storedResources"));
			std::string item;

			// Para cada tipo separado por coma
			while (std::getline(ss, item, ',')) {
				if (item.size()>0)
					_storedResources[ResourcesManager::parseResourceType(item)] = 0;
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
				_providedResources.emplace(ResourcesManager::parseResourceType(item));
			}
		}

		return true;
	} // spawn

	void CResourceBuilding::tick(unsigned int msecs){
		IComponent::tick(msecs);
	} // tick

	bool CResourceBuilding::HandleMessage(const ResourceMessage& msg){
		if (msg._type != MessageType::RESOURCES_CHANGE)
			return false;

		// Si el tipo de recurso es aceptado por el componente
		if (_storedResources.count(msg._resourceType) > 0){
			int quantityChanged = msg._change;

			// Si nos quieren quitan más de lo que tenemos
			if ((quantityChanged < 0) && ((_storedResources[msg._resourceType] + quantityChanged) < 0)){
				std::cout << "Resource quantity to remove is higher than available" << std::endl;
				quantityChanged = -_storedResources[msg._resourceType];
			}

			// Registramos el cambio
			_storedResources[msg._resourceType] += quantityChanged;
			std::cout << "Resource(" << msg._resourceType << ")=" << _storedResources[msg._resourceType] << std::endl;

			// Notificamos al ResourcesManager
			// TODO Atajo temporal para obtener el ResourcesManager
			Logic::ResourcesManager *resourcesManager = ((Application::CGameState*) Application::CGaleonApplication::getSingletonPtr()->getState())->getResourcesManager();
			resourcesManager->increaseResources(msg._resourceType, quantityChanged);
		}

		else{
			assert(false && "Resource type not handled");
			return false;
		}

		return true;
	}

}