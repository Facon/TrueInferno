#ifndef PLACEABLE_H_
#define PLACEABLE_H_

#include <vector>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/PlaceableType.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/TerrainType.h"

#include <unordered_set>


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
	class Tile;
	class CTileManager;
}

namespace Logic {
	class CPlaceable : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CPlaceable);

	// Anotamos la clase como friend de BuildingManager para permitirle acceso al tipo de edificio y de placeable
	friend class CBuildingManager;

	public:
		/**
		Constructor por defecto.
		*/
		CPlaceable();

		/**
		Destructor
		*/
		virtual ~CPlaceable();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Float this placeable to a given logic position*/
		void floatTo(const Vector3 newOriginPosition, bool showFloating);

		/** Coloca el Placeable en la posición original */
		bool place();

		/** Gets occupied tiles depending on the placeable's floor */
		const std::vector<Tile*> getTiles() const;

		/** Gets all adyacent tiles to the placeable's own tiles */
		const std::unordered_set<Tile*> getAdyacentTiles() const;

		/** Checks if it's possible to place all the placeable's floor tiles starting at given origin position. All tiles must be entity free. 
		Returns true if placement is possible, false otherways */
		bool checkPlacementIsPossible(const Vector3 &originPosition) const;

		/** Devuelve un boolean indicando si es posible hacer pasar una SoulPath por el Placeable */
		bool canPassSoulPath() const;

		/** Devuelve un boolean indicando si es posible hacer caminar un alma por el Placeable */
		bool canPassWalkingSoul() const;

		/** Devuelve un boolean indicando si es posible que el Placeable se coloque sobre terreno de recurso */
		bool canBePlacedOnTerrain(TerrainType terrainType) const;

		/** Devuelve un boolean a true si el Placeable corresponde a un edificio */
		bool isBuilding() const;

		virtual bool HandleMessage(const MovePlaceableMessage& msg);

		//virtual bool HandleMessage(const WalkSoulPathMessage& msg);

		virtual bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);

	private:
		/** Altura añadida a la posición del Placeable para que parezca que está justo encima */
		const float HEIGHT_ON_TILE = 1.0f;

		/** Altura añadida a la posición del Placeable para que parezca que flota por encima */
		const float HEIGHT_FLOATING_OVER_TILE = 1.5f;

		/** Floor's absolute logic origin position in the matrix */
		Vector3 _floorOriginPosition;

		/** Flag set to true when logic position has changed */
		//bool _logicPositionChanged;

		/** Floor's x size */
		int _floorX;

		/** Floor's z size */
		int _floorZ;

		/** Floor's occupied tiles */
		std::vector<Tile*> _tiles;

		/** Tiles adyacent to floor's occupied tiles */
		std::unordered_set<Tile*> _adyacentTiles;

		/** Flag a true si el Placeable está flotando y, por tanto, todavía no debe estar registrado ni colisionar con otros objetos */
		bool _floating;

		/** Tile manager reference */
		CTileManager* _tileManager;

		/** Tipo del placeable */
		Logic::PlaceableType _placeableType;

		/** Tipo del edificio */
		Logic::BuildingType _buildingType;

		/** Tipos de terrenos aceptados */
		std::unordered_set<Logic::TerrainType> _validTerrainTypes;

		/** Objetivo para calcular ruta */
		CPlaceable* _walkingSoulTarget;

		/** Actualiza el conjunto de tiles adyacentes */
		void updateAdyacentTiles();

		/** Procesa peticiones pendientes de cálculo de rutas */
		void processWalkingSoulPathRequest();

		/** Flag para comprobar si debe o no lanzar el evento del tutorial asociado a esta condición */
		bool _firstBuilding = true;

	protected:
		/** Parsea un enum PlaceableType a partir del nombre en texto
		TODO Mover junto al código del enum */
		static PlaceableType parsePlaceableType(const std::string& name);

		/** Parsea un enum BuildingType a partir del nombre en texto
		TODO Mover junto al código del enum */
		static BuildingType parseBuildingType(const std::string& name);

		/** Parsea un enum TerrainType a partir del nombre en texto
		TODO Mover junto al código del enum */
		static TerrainType parseTerrainType(const std::string& name);

		// Devuelve el PlaceableType de este placeable
		PlaceableType CPlaceable::getPlaceableType(){
			return _placeableType;
		}

		// Devuelve el BuildingType de este placeable
		BuildingType CPlaceable::getBuildingType(){
			return _buildingType;
		}

	}; // class Placeable

	REG_FACTORY(CPlaceable);

} // namespace Logic

#endif // PLACEABLE_H_