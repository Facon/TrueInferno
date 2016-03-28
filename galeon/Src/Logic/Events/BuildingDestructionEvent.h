//---------------------------------------------------------------------------
// BuildingDestructionEvent.h
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.h

Declaración de un evento de tipo INFO consistente en la destrucción de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_BuildingDestructionEvent_H
#define __Logic_BuildingDestructionEvent_H

#include "Event.h"

// Predeclaración de clases para ahorrar tiempo de compilación.
namespace Logic
{
}

/**
Namespace que engloba la lógica del juego. Engloba desde el mapa lógico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para más información ver @ref LogicGroup).

@author David Llansó
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Clase que representa un evento de destrucción de un edificio
	de forma aleatoria.

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CBuildingDestructionEvent : public CEvent
	{

	public:

		/**
		Constructor.
		*/
		CBuildingDestructionEvent(unsigned int time) : CEvent(INFO, time) {};

		/**
		Destructor.
		*/
		~CBuildingDestructionEvent() {};

	protected:

		/**
		Método encargado únicamente de ejecutar la lógica específica
		asociada al evento.
		*/
		void execute();
		
	}; // class CBuildingDestructionEvent

} // namespace Logic

#endif // __Logic_BuildingDestructionEvent_H
