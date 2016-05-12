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
		Constructor basado en trigger.
		*/
		CEndGameEvent(bool victory) :
			CEvent(INFO, CEvent::ConditionTriggerType::END_GAME), _victory(victory)	{};

		/**
		Constructor basado en tiempo.

		@param victory Flag a true si el resultado de la ronda es de victoria para el jugador, a false para derrota
		@param delay Retraso en ms para lanzar el evento desde el instante de tiempo actual
		*/
		CEndGameEvent(bool victory, int delay) :
			CEvent(INFO, delay, false), _victory(victory) {};

		/**
		Destructor.
		*/
		~CEndGameEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Resultado de la partida (Victoria o Derrota).
		*/
		bool _victory;

		/**
		M�todo encargado �nicamente de ejecutar la l�gica espec�fica
		asociada al evento.
		*/
		void execute();
		
	}; // class CEndGameEvent

} // namespace Logic

#endif // __Logic_EndGameEvent_H
