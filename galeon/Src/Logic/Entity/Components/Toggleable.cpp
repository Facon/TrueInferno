#include "Toggleable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	RTTI_ROOT_IMPL(CToggleable);
	IMP_FACTORY(CToggleable);

	bool CToggleable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if (entityInfo->hasAttribute("initiallyEnabled")){
			_enabled = entityInfo->getBoolAttribute("initiallyEnabled");
		}
		else
			_enabled = false;

		_update = false;
		_newEnabled = _enabled;

		return true;
	} // spawn

	void CToggleable::tick(unsigned int msecs){
		// Si hay que actulizar
		if (_update){
			// Actualizamos
			_enabled = _newEnabled;

			std::cout << "Logic enabled = " << _enabled << std::endl;

			// Limpiamos el flag de actualización
			_update = false;
		}
		
	} // tick

	/** Devuelve si la entidad está habilitada o no a nivel lógico */
	bool CToggleable::isLogicEnabled() const {
		return _enabled;
	}

	bool CToggleable::HandleMessage(const ToggleMessage& msg){
		// Si no había actualización en curso
		if (!_update){
			// El nuevo valor de habilitado viene dado por el mensaje
			_newEnabled = msg._enabled;
			
			// Marcamos que estamos actualizando
			_update = true;
		}

		// Si sí había actualización en curso
		else{
			// Habilitaremos si y sólo todos los cambios son para habilitar
			_newEnabled &= msg._enabled;
		}
		
		return true;
	}


} // namespace Logic