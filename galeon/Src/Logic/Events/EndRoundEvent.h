//---------------------------------------------------------------------------
// EndRoundEvent.h
//---------------------------------------------------------------------------

/**
@file EndRoundEvent.h

Declaraci�n de un evento de tipo INFO que muestra al jugador
el desenlace de la ronda (victoria o derrota).

@see Logic::CEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef END_ROUND_EVENT_H_
#define END_ROUND_EVENT_H_

#include "Event.h"

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
	Clase que representa un evento de fin de ronda para
	mostrar el panel de victoria o derrota.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Mayo, 2016
	*/
	class CEndRoundEvent : public CEvent
	{

	public:

		/**
		Constructor basado en trigger.
		*/
		CEndRoundEvent(bool victory) :
			CEvent(INFO, CEvent::ConditionTriggerType::END_ROUND), _victory(victory) {};

		/**
		Constructor basado en tiempo.

		@param victory Flag a true si el resultado de la partida es de victoria para el jugador, a false para derrota
		@param delay Retraso en ms para lanzar el evento desde el instante de tiempo actual
		*/
		CEndRoundEvent(bool victory, int delay) :
			CEvent(INFO, delay, false), _victory(victory) {};

		/**
		Destructor.
		*/
		~CEndRoundEvent() {};

	protected:

		/**
		Resultado de la ronda (Victoria o Derrota).
		*/
		bool _victory;

		/**
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		void execute();
		
	}; // class CEndRoundEvent

} // namespace Logic

#endif // END_ROUND_EVENT_H_
