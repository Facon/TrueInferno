#ifndef TILE_H_
#define TILE_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Entity/TerrainType.h"
#include "Placeable.h"

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

		/** Destructor */
		Tile::~Tile();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

        /*
		// Aceptación de mensajes
		virtual bool accept(const MessageType &message);

		// Procesamiento de mensajes
		virtual void process(const MessageType &message);
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

		/** Setter for entityAbove */
		void setPlaceableAbove(CPlaceable* placeableAbove);

		/** Getter for entityAbove */
		const CPlaceable* getPlaceableAbove() const;

		/** Comprueba si la Tile permite colocar encima un placeable dado */
		bool allowsPlaceable(const CPlaceable *placeable) const;

		/** Devuelve el vector de tiles adyacentes */
		const std::vector<Tile*>* getAdjacentTiles();

		/** Devuelve un boolean indicando si es posible hacer pasar una SoulPath por el Tile */
		bool canPassSoulPath();

		/** Devuelve un boolean indicando si es posible hacer caminar un alma por el Tile */
		bool canPassWalkingSoul();

		/** Imprime información de debug */
		void printDebugInfo();


		/** Calcula la distancia de Manhattan con respecto a otro Tile.
		Debería estar en OgreVector3 para generalizar su uso pero no linka.
		*/
		inline float manhattanDistance(const Tile& rhs) const
		{
			return 
				Ogre::Math::Abs(this->_logicPosition.x - rhs._logicPosition.x) + 
				Ogre::Math::Abs(this->_logicPosition.y - rhs._logicPosition.y) + 
				Ogre::Math::Abs(this->_logicPosition.z - rhs._logicPosition.z);
		}

	private:
		/** Número de tiles adyacentes que tiene normalmente un tile (salvo las de los extremos): 4 (si no se consideran las diagonales) */
		static const int NUM_ADJACENT = 4;

		/** Logic position in the matrix */
		Vector3 _logicPosition;

		TerrainType _terrainType;

		/** Vector de tiles adyacentes */
		std::vector<Tile*>* _adjacentTiles;

		/** Referencia al TileManager */
		CTileManager *_tileManager;

		/** Entity (i.e. building, road) placed above */
		CPlaceable *_placeableAbove;
	}; // class Tile

	REG_FACTORY(Tile);

} // namespace Logic

#endif // TILE_H_