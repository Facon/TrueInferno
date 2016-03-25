//---------------------------------------------------------------------------
// BuildingDestructionEvent.h
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.h

Declaraci�n de un evento de tipo INFO consistente en la destrucci�n de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_BuildingDestructionEvent_H
#define __Logic_BuildingDestructionEvent_H

#include "Event.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n.
namespace Logic
{
}

/**
Namespace que engloba la l�gica del juego. Engloba desde el mapa l�gico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para m�s informaci�n ver @ref LogicGroup).

@author David Llans�
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Clase que representa un evento de destrucci�n de un edificio
	de forma aleatoria.

	@ingroup logicGroup

	@author Ra�l Segura
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
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		void execute();
		
	}; // class CBuildingDestructionEvent

} // namespace Logic

#endif // __Logic_BuildingDestructionEvent_H
