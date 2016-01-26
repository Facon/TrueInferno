#ifndef PLACEABLE_H_
#define PLACEABLE_H_

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class Tile;
}

namespace Logic {
	class Placeable : public IComponent{
		DEC_FACTORY(Placeable);

	public:
		/**
		Constructor por defecto.
		*/
		Placeable();

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

		/** Place this placeable on the given logic position*/
		bool place(const Vector3 logicPosition);

		/** Gets occupied tiles depending on the placeable's floor */
		const std::vector<Tile*> getTiles();

	private:
		/** Floor's absolute logic origin position in the matrix */
		Vector3 _floorOriginPosition;

		/** Floor's relative positions to the origin */
		std::vector<Vector3> _floorRelativePositions;

		/** Floor's occupied tiles */
		std::vector<Tile*> _tiles;

		/** Obtains position attribute name */
		std::string getPositionAttributeName(int num);
	}; // class Placeable

	REG_FACTORY(Placeable);

} // namespace Logic

#endif // PLACEABLE_H_