#ifndef TILE_H_
#define TILE_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Maps/Managers/TileManager.h"

#include <vector>

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
		void setEntityAbove(CEntity *entityAbove);

		/** Getter for entityAbove */
		const CEntity* getEntityAbove();

		/** Devuelve el vector de tiles adyacentes */
		const std::vector<Tile*> getAdjacentTiles();

		/** Devuelve un boolean indicando si es posible construir SoulPath en el Tile */
		bool canBuildSoulPath();

		/** Devuelve un boolean indicando si es posible construir un edificio en el Tile */
		bool canBuildBuilding();

	private:
		/** Número de tiles adyacentes que tiene normalmente un tile (salvo las de los extremos): 4 (si no se consideran las diagonales) */
		static const int NUM_ADJACENT = 4;

		/** Logic position in the matrix */
		Vector3 _logicPosition;

		TerrainType _terrainType;

		/** Entity placed above */
		CEntity *_entityAbove;

		/** Vector de tiles adyacentes */
		std::vector<Tile*> _adjacentTiles;

		/** Referencia al TileManager */
		CTileManager *_tileManager;
	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_