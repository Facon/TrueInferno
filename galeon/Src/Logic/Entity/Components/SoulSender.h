#ifndef SOUL_SENDER_H_
#define SOUL_SENDER_H_

#include "Logic/Entity/Component.h"

namespace Logic {
	class CSoulSender : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CSoulSender);

	public:
		/**
		Constructor por defecto.
		*/
		CSoulSender();

		virtual ~CSoulSender();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

	private:
		const float SOUL_ON_TILE_HEIGHT = 2.0;

	}; // class CSoulSender

	REG_FACTORY(CSoulSender);

} // namespace Logic

#endif // SOUL_SENDER_H_