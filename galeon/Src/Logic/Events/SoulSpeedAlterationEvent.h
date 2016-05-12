//---------------------------------------------------------------------------
// SoulSpeedAlterationEvent.h
//---------------------------------------------------------------------------

/**
@file SoulSpeedAlterationEvent.h

Declaración de un evento de tipo INFO consistente en alterar la velocidad
de movimiento y de recolección de recursos de las almas durante un cierto
tiempo.

@see Logic::CEvent
@see Logic::CSoulsSpeedReductionEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#ifndef SOUL_SPEED_ALTERATION_EVENT_H_
#define SOUL_SPEED_ALTERATION_EVENT_H_

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
	Clase que representa un evento de alteración de todas las almas
	de la ciudad.

	@ingroup logicGroup

	@author Raúl Segura
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
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Este evento no es instantáneo, sino que tiene un efecto prolongado
		en el tiempo, por lo que se usará este flag para diferenciar si
		debe aplicarse el evento (en este caso, la alteración de las
		almas) o restaurar la situación habitual de juego.
		*/
		bool _restore;

		/**
		Método encargado de ejecutar el evento.
		*/
		void execute();

		/**
		Método encargado de llevar a cabo la lógica específica asociada
		al evento.
		*/
		void apply();

		/**
		Método encargado de restaurar, a su valor habitual, la lógica
		modificada previamente por el evento.
		*/
		void restore();

	private:
		/**
		Factor por el cual se altera la velocidad de las almas. 
		Si factor < 1.0, el evento ralentiza. Si factor > 1, el evento acelera.
		*/
		float _factor;

		/** Duración (ms) de la alteración de velocidad */
		int _duration;
		
	}; // class CSoulSpeedAlterationEvent

} // namespace Logic

#endif // SOUL_SPEED_ALTERATION_EVENT_H_
