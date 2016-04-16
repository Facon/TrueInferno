#include "Toggleable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	RTTI_ROOT_IMPL(CToggleable);
	IMP_FACTORY(CToggleable);

	bool CToggleable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		/*
		if (entityInfo->hasAttribute("initiallyEnabled")){
			_enabled = entityInfo->getBoolAttribute("initiallyEnabled");
		}
		else
			_enabled = false;

		_update = false;
		_newEnabled = _enabled;
		*/

		// Guardamos el set con los requisitos de la entidad (si hay alguno)
		if (entityInfo->hasAttribute("requirements")){
			std::istringstream ss(entityInfo->getStringAttribute("requirements"));
			std::string item;

			// Para cada elemento separado por coma
			while (std::getline(ss, item, ',')) {
				// Almacenamos el requisito
				_requirements.emplace(parseLogicRequirement(item));
			}
		}

		return true;
	} // spawn

	void CToggleable::tick(unsigned int msecs){
		/*
		// Si hay que actualizar
		if (_update){
			// Actualizamos
			_enabled = _newEnabled;

			std::cout << "Logic enabled = " << _enabled << std::endl;

			// Limpiamos el flag de actualización
			_update = false;
		}
		*/
	} // tick

	bool CToggleable::isLogicEnabled(const std::set<LogicRequirement>& skippedRequirements) const {
		// OJO con confundir el argumento del set skippedRequirements con el propio _skippedRequirements que tiene el componente CToggleable!

		// Si no hay requisitos por satisfacer estamos habilitados
		if (_requirements.empty())
			return true;

		// Si el conjunto de requisitos a ignorar contiene el elemento especial 'All' estamos habilitados
		if (skippedRequirements.count(LogicRequirement::All) > 0)
			return true;

		// Buscamos si hay algún requisito pendiente que NO esté entre los ignorados y fallamos
		for (auto it = _requirements.cbegin(); it != _requirements.cend(); ++it){
			LogicRequirement requirement = *it;
			if (skippedRequirements.count(requirement) == 0)
				return false;
		}
		
		// True porque todos los requisitos pendientes están ignorados
		return true;
	}


	bool CToggleable::HandleMessage(const ToggleMessage& msg){
		/*
		// Si no había actualización en curso
		if (!_update){
			// El nuevo valor de habilitado viene dado por el mensaje
			_newEnabled = msg._enable;
			
			// Marcamos que estamos actualizando
			_update = true;
		}

		// Si sí había actualización en curso
		else{
			// Habilitaremos si y sólo todos los cambios son para habilitar
			_newEnabled &= msg._enable;
		}
		*/

		// Si tenemos que añadir requisito
		if (msg._add) {
			bool added = (_requirements.emplace(msg._requirement)).second;
			assert(added && "Adding requirement more than once");

			std::cout << "Logic: Added '" << printLogicRequirement(msg._requirement) << "' requirement" << std::endl;
		}

		// Si no, tenemos que eliminar requisito
		else {
			bool erased = _requirements.erase(msg._requirement) != 0;
			assert(erased && "Deleting requirement that doesn't exist");

			std::cout << "Logic: Removed '" << printLogicRequirement(msg._requirement) << "' requirement. " << _requirements.size() << " requirements left" << std::endl;
		}

		return true;
	}


} // namespace Logic