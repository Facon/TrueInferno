//---------------------------------------------------------------------------
// EndGameEvent.h
//---------------------------------------------------------------------------

/**
@file EndGameEvent.h

Declaraci�n de un evento de tipo INFO consistente �nicamente en mostrar
el panel de fin del juego (con victoria o derrota).

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_EndGameEvent_H
#define __Logic_EndGameEvent_H

#include "Event.h"
#include "ConditionEvents.h"

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
	Clase que representa un evento de fin del juego encargado simplemente
	de mostrar el panel final de victoria o derrota.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Marzo, 2016
	*/
	class CEndGameEvent : public CEvent
	{

	public:

		/**
		Constructor.
		*/
		CEndGameEvent(bool victory) :
			CEvent(INFO, ConditionEventType::END_GAME), _victory(victory)	{};

		/**
		Destructor.
		*/
		virtual ~CEndGameEvent() {};

	protected:

		/**
		Resultado de la partida (Victoria o Derrota).
		*/
		bool _victory;

		/**
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		virtual void execute();
		
	}; // class CEndGameEvent

} // namespace Logic

#endif // __Logic_EndGameEvent_H
