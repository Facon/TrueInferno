//---------------------------------------------------------------------------
// SoulsSpeedReductionEvent.h
//---------------------------------------------------------------------------

/**
@file SoulsSpeedReductionEvent.h

Declaraci�n de un evento de tipo INFO consistente en ralentizar la velocidad
de movimiento y de recolecci�n de recursos de las almas durante un cierto
tiempo.

@see Logic::CEvent

@author Ra�l Segura
@date Abril, 2016
*/

#ifndef __Logic_SoulsSpeedReductionEvent_H
#define __Logic_SoulsSpeedReductionEvent_H

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
	Clase que representa un evento de ralentizaci�n de todas las almas
	de la ciudad.

	@ingroup logicGroup

	@author Ra�l Segura
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
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

	protected:

		/**
		Este evento no es instant�neo, sino que tiene un efecto prolongado
		en el tiempo, por lo que se usar� este flag para diferenciar si
		debe aplicarse el evento (en este caso, la ralentizaci�n de las
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
		
	}; // class CSoulsSpeedReductionEvent

} // namespace Logic

#endif // __Logic_SoulsSpeedReductionEvent_H
