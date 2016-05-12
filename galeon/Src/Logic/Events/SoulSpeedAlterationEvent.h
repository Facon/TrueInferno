//---------------------------------------------------------------------------
// SoulSpeedAlterationEvent.h
//---------------------------------------------------------------------------

/**
@file SoulSpeedAlterationEvent.h

Declaraci�n de un evento de tipo INFO consistente en alterar la velocidad
de movimiento y de recolecci�n de recursos de las almas durante un cierto
tiempo.

@see Logic::CEvent
@see Logic::CSoulsSpeedReductionEvent

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef SOUL_SPEED_ALTERATION_EVENT_H_
#define SOUL_SPEED_ALTERATION_EVENT_H_

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
	Clase que representa un evento de alteraci�n de todas las almas
	de la ciudad.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Abril, 2016
	*/
	class CSoulSpeedAlterationEvent : public CEvent
	{

	public:

		/**
		Constructor.
		*/
		CSoulSpeedAlterationEvent(unsigned long time, float factor, int duration, bool absoluteTime = true, bool restore = false) :
			CEvent(INFO, time, absoluteTime), _factor(factor), _duration(duration), _restore(restore) {};

		static CSoulSpeedAlterationEvent* addCSoulSpeedAlterationEvent(unsigned long time, float factor, int duration, bool absoluteTime = true, bool restore = false) {
			return new CSoulSpeedAlterationEvent(time, factor, duration, absoluteTime, restore);
		}

		/**
		Destructor.
		*/
		~CSoulSpeedAlterationEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Este evento no es instant�neo, sino que tiene un efecto prolongado
		en el tiempo, por lo que se usar� este flag para diferenciar si
		debe aplicarse el evento (en este caso, la alteraci�n de las
		almas) o restaurar la situaci�n habitual de juego.
		*/
		bool _restore;

		/**
		M�todo encargado de ejecutar el evento.
		*/
		void execute();

		/**
		M�todo encargado de llevar a cabo la l�gica espec�fica asociada
		al evento.
		*/
		void apply();

		/**
		M�todo encargado de restaurar, a su valor habitual, la l�gica
		modificada previamente por el evento.
		*/
		void restore();

	private:
		/**
		Factor por el cual se altera la velocidad de las almas. 
		Si factor < 1.0, el evento ralentiza. Si factor > 1, el evento acelera.
		*/
		float _factor;

		/** Duraci�n (ms) de la alteraci�n de velocidad */
		int _duration;
		
	}; // class CSoulSpeedAlterationEvent

} // namespace Logic

#endif // SOUL_SPEED_ALTERATION_EVENT_H_
