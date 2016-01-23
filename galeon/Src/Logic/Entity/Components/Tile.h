#ifndef TILE_H_
#define TILE_H_

#include "Logic/Entity/Component.h"
#include "Logic/Maps/Managers/TileManager.h"

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

		/** 
		Returns logic position
		*/
		const Vector3 getLogicPosition();

		void setTerrainType(const TerrainType &terrainType);

		const TerrainType getTerrainType();

	private:
		/** Logic position in the matrix */
		Vector3 _logicPosition;

		TerrainType _terrainType;
	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_