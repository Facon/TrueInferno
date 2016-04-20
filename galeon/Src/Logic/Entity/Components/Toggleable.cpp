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

		// Guardamos el set con los requisitos iniciales de la entidad (si hay alguno)
		if (entityInfo->hasAttribute("requirements")){
			std::istringstream ss(entityInfo->getStringAttribute("requirements"));
			std::string item;

			// Para cada elemento separado por coma
			while (std::getline(ss, item, ',')) {
				// Almacenamos el requisito
				addRequirement(parseLogicRequirement(item));
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
		// Sólo se aceptan solicitudes de cambio
		if (msg._type != MessageType::TOGGLE_REQUEST)
			return false;

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
			addRequirement(msg._requirement);
		}

		// Si no, tenemos que eliminar requisito
		else {
			removeRequirement(msg._requirement);
		}

		return true;
	}

	bool CToggleable::addRequirement(LogicRequirement requirement){
		// Chequeamos si es el primero en añadirse
		bool wasEmpty = false;
		if (_requirements.empty()){
			wasEmpty = true;
		}

		// Chequeamos que un mismo requisito no se intente añadir varias veces
		bool added = (_requirements.emplace(requirement)).second;
		assert(added && "Adding requirement more than once");

		// Si se ha añadido algo y la lista de requisitos estaba vacía: Fue el primero en añadirse ==> La entidad acaba de ser desactivada lógicamente
		if (added && wasEmpty){
			ToggleMessage m(false);
			assert(m.Dispatch(*_entity) && "Unhandled ToggleMessage to disable entity");
		}

		//std::cout << "Logic: Added '" << printLogicRequirement(requirement) << "' requirement" << std::endl;

		return added;
	}

	bool CToggleable::removeRequirement(LogicRequirement requirement){
		// Chequeamos que un requisito exista antes de intentar eliminarlo
		bool removed = _requirements.erase(requirement) != 0;
		assert(removed && "Removing requirement that doesn't exist");

		// Si se ha borrado algo y la lista de requisitos ahora está vacía: Fue el último por eliminar ==> La entidad acaba de ser activada lógicamente
		if (removed && _requirements.empty()){
			ToggleMessage m(true);
			assert(m.Dispatch(*_entity) && "Unhandled ToggleMessage to enable entity");
		}

		//std::cout << "Logic: Removed '" << printLogicRequirement(requirement) << "' requirement. " << _requirements.size() << " requirements left" << std::endl;

		return removed;
	}


} // namespace Logic