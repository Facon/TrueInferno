//---------------------------------------------------------------------------
// SoulsSpeedReductionEvent.h
//---------------------------------------------------------------------------

/**
@file SoulsSpeedReductionEvent.h

Declaración de un evento de tipo INFO consistente en ralentizar la velocidad
de movimiento y de recolección de recursos de las almas durante un cierto
tiempo.

@see Logic::CEvent

@author Raúl Segura
@date Abril, 2016
*/

#ifndef __Logic_SoulsSpeedReductionEvent_H
#define __Logic_SoulsSpeedReductionEvent_H

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
	Clase que representa un evento de ralentización de todas las almas
	de la ciudad.

	@ingroup logicGroup

	@author Raúl Segura
	@date Abril, 2016
	*/
	class CSoulsSpeedReductionEvent : public CEvent
	{

	public:

		/**
		Constructores.
		*/
		CSoulsSpeedReductionEvent(unsigned long time, bool restore = false) :
			CEvent(INFO, time), _restore(restore) {};

		CSoulsSpeedReductionEvent(ConditionTriggerType condEventType, bool restore = false, bool dummy = true) :
			CEvent(INFO, condEventType), _restore(restore) {};

		/**
		Destructor.
		*/
		~CSoulsSpeedReductionEvent() {};

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Este evento no es instantáneo, sino que tiene un efecto prolongado
		en el tiempo, por lo que se usará este flag para diferenciar si
		debe aplicarse el evento (en este caso, la ralentización de las
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
		
	}; // class CSoulsSpeedReductionEvent

} // namespace Logic

#endif // __Logic_SoulsSpeedReductionEvent_H
