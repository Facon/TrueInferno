#ifndef PLACEABLE_H_
#define PLACEABLE_H_

#include <vector>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/*
		//Aceptaci�n de mensajes
		virtual bool accept(const TMessage &message);

		//Procesamiento de mensajes
		virtual void process(const TMessage &message);
		*/

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Place this placeable on a given logic position*/
		bool place(const Vector3 newOriginPosition);

		/** Gets occupied tiles depending on the placeable's floor */
		const std::vector<Tile*> getTiles();

		/** Checks if it's possible to place all the placeable's floor tiles starting at given origin position. All tiles must be entity free. 
		Returns true if placement is possible, false otherways */
		bool checkPlacementIsPossible(const Vector3 &originPosition);

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

	}; // class Placeable

	REG_FACTORY(CPlaceable);

} // namespace Logic

#endif // PLACEABLE_H_