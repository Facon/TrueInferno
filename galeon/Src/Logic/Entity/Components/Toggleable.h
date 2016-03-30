#ifndef TOGGLEABLE_H_
#define TOGGLEABLE_H_

#include "Logic/Entity/Component.h"

namespace Logic {
	/** Componente que controla el habilitado y deshabilitado a nivel l�gico de la entidad */
	class CToggleable : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CToggleable);

	public:
		/**
		Constructor por defecto.
		*/
		CToggleable() : _enabled(false) {}

		/**
		Destructor por defecto.
		*/
		virtual ~CToggleable() {}

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		virtual bool HandleMessage(const ToggleMessage& msg);

		/** Devuelve si la entidad est� habilitada o no a nivel l�gico */
		bool isLogicEnabled() const;

	private:
		/** Flag que indica si la entidad est� habilitada o no a nivel l�gico */
		bool _enabled;

		/** Flag para indicar que hay que actualizar el flago de habilitado l�gico */
		bool _update;

		/** Valor nuevo de habilitado l�gico para el siguiente tick */
		bool _newEnabled;
		
	}; // class CToggleable

	REG_FACTORY(CToggleable);

} // namespace Logic

#endif // TOGGLEABLE_H_