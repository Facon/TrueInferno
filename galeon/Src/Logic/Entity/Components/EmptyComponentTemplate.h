/*
#ifndef TILE_H_
#define TILE_H_

#include "Logic/Entity/Component.h"

namespace Logic {
	class CTile : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CTile);

	public:
		/**
		Constructor por defecto.
		/
		CTile();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		/
		virtual void tick(unsigned int msecs);

	private:

	}; // class CTile

	REG_FACTORY(CTile);

} // namespace Logic

#endif // TILE_H_
*/