#ifndef PLACEABLE_H_
#define PLACEABLE_H_

#include <vector>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/PlaceableType.h"
#include "Logic/Entity/BuildingType.h"

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

	public:
		/**
		Constructor por defecto.
		*/
		CPlaceable();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Place this placeable on a given logic position*/
		bool place(const Vector3 newOriginPosition);

		/** Gets occupied tiles depending on the placeable's floor */
		const std::vector<Tile*> getTiles();

		/** Checks if it's possible to place all the placeable's floor tiles starting at given origin position. All tiles must be entity free. 
		Returns true if placement is possible, false otherways */
		bool checkPlacementIsPossible(const Vector3 &originPosition);

		/** Devuelve un boolean indicando si es posible hacer pasar una SoulPath por el Placeable */
		bool canPassSoulPath();

		/** Devuelve un boolean indicando si es posible hacer caminar un alma por el Placeable */
		bool canPassWalkingSoul();

		/** Devuelve un boolean a true si el Placeable corresponde a un edificio */
		bool isBuilding();

		virtual bool CPlaceable::HandleMessage(const PlaceMessage& msg);

	private:
		/** Height added to entities placed over tiles so they don't overlap
		TODO Should be relative to a tile's height
		*/
		const float HEIGHT_OVER_TILE = 1.0;

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

		/** Tile manager reference */
		CTileManager* _tileManager;

		/** Tipo del placeable */
		Logic::PlaceableType _placeableType;

		/** Tipo del edificio */
		Logic::BuildingType _buildingType;

		PlaceableType CPlaceable::getPlaceableType(std::string name);

		BuildingType CPlaceable::getBuildingType(std::string name);

	}; // class Placeable

	REG_FACTORY(CPlaceable);

} // namespace Logic

#endif // PLACEABLE_H_