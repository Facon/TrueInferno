#ifndef TILE_H_
#define TILE_H_

#include "Logic/Entity/Component.h"

namespace Logic {
	class Tile : public IComponent{
		DEC_FACTORY(Tile);

	public:
		/**
		Constructor por defecto.
		*/
		Tile();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Aceptación de mensajes
		*/
		virtual bool accept(const TMessage &message);

		/**
		Procesamiento de mensajes
		*/
		virtual void process(const TMessage &message);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

	private:

	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_