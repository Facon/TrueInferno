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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Aceptaci�n de mensajes
		*/
		virtual bool accept(const TMessage &message);

		/**
		Procesamiento de mensajes
		*/
		virtual void process(const TMessage &message);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

	private:

	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_