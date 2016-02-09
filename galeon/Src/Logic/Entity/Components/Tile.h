#ifndef TILE_H_
#define TILE_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Placeable.h"

namespace Logic {
	class Tile : public IComponent{
		RTTI_DECL;
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

        /*
		// Aceptación de mensajes
		virtual bool accept(const TMessage &message);

		// Procesamiento de mensajes
		virtual void process(const TMessage &message);
        */
		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** 
		Returns logic position
		*/
		const Vector3 getLogicPosition();

		/** Setter for terrainType */
		void setTerrainType(const TerrainType &terrainType);

		/** Getter for terrainType */
		const TerrainType getTerrainType();

		/** Setter for entityAbove */
		void setPlaceableAbove(CPlaceable* placeableAbove);

		/** Getter for entityAbove */
		const CPlaceable* getPlaceableAbove();

		/** Checks if tile allows placing something on it (i.e. if the tile has no other Placeable on it)
		It could be extended to check if some specific building can be placed (e.g. can a regular building be placed on a resource tile?) */
		bool canPlaceSomething();

	private:
		/** Logic position in the matrix */
		Vector3 _logicPosition;

		TerrainType _terrainType;

		/** Entity (i.e. building, road) placed above */
		CPlaceable *_placeableAbove;
	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_