//---------------------------------------------------------------------------
// Event.h
//---------------------------------------------------------------------------

/**
@file Event.h

Evento de juego. Los eventos se dividir�n de dos formas posibles: en funci�n
de su tipo, como eventos de informaci�n y eventos de decisi�n, y en funci�n
de su trigger, como eventos lanzados por tiempo y por condici�n/acci�n.

@see Logic::CEventManager

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_Event_H
#define __Logic_Event_H

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n.
namespace Logic
{
	enum ConditionTriggerType;
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
	Clase abstracta ra�z de la jerarqu�a de eventos del juego.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Marzo, 2016
	*/
	class CEvent
	{

	public:

		/**
		Posibles tipos de eventos.
		*/
		enum EventType
		{
			// Eventos habituales del juego. Normalmente informativos,
			// ya sea sobre acciones pasadas, para mantener al jugador
			// informado sobre sucesos importantes, o futuras, a las
			// que pueda o deba reaccionar de una forma u otra.
			INFO,
			// Eventos de decisi�n que obligar�n al jugador a elegir
			// entre 2 o m�s opciones, provocando cada una de ellas
			// un resultado diferente sobre la partida.
			DECISION
		};

		/**
		Posibles triggers de eventos.
		*/
		enum EventTrigger
		{
			// Eventos lanzados al llegar a un tiempo concreto de juego.
			TIME,
			// Eventos lanzados cuando se cumple una determinada
			// condici�n durante la partida.
			CONDITION
		};

		/**
		Posibles tipos de triggers CONDITION.
		IMPORTANTE: No cambiar el valor entero asociado a cada valor del
		enumerado y a�adir cada nuevo valor en CEvent::luaRegister().
		*/
		enum ConditionTriggerType
		{
			// Eventos disparados como parte del tutorial para guiar al
			// jugador durante los primeros instantes de la partida.
			TUTORIAL,
			// Eventos disparados al final de la partida para mostrar el
			// resultado de la misma y la puntuaci�n final.
			END_GAME
		};

		/**
		Constructores.
		*/
		CEvent(EventType type, unsigned long time) :
			_type(type), _trigger(TIME), _time(time) {}

		CEvent(EventType type, ConditionTriggerType conditionType) :
			_type(type), _trigger(CONDITION), _conditionType(conditionType) {}

		/**
		Destructor.
		*/
		virtual ~CEvent() {}

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

		/**
		Getters.
		*/
		EventType getEventType() { return _type; }
		EventTrigger getEventTrigger() { return _trigger; }
		ConditionTriggerType getConditionTriggerType() { return _conditionType; }

		unsigned long getTime() { return _time; }

		/**
		Comprueba si debe lanzar el evento y lo hace en caso positivo.
		En caso negativo, simplemente no hace nada.

		@return true si el evento fue lanzado.
		*/
		bool launch();

	protected:

		/**
		Tipo del evento.
		*/
		EventType _type;

		/**
		Trigger del evento.
		*/
		EventTrigger _trigger;

		/**
		Tipo de condici�n que lanza el evento.
		NULL si _trigger != CONDITION.
		*/
		ConditionTriggerType _conditionType;

		/**
		Tiempo de lanzamiento del evento en milisegundos.
		0 si _trigger != TIME.
		*/
		unsigned long _time = 0;

		/**
		Comprueba si el evento debe ser lanzado atendiendo a su trigger.
		*/
		bool mustBeLaunched();

		/**
		M�todo abstracto que ejecuta la l�gica espec�fica asociada al
		evento. Deber� ser sobreescrito por cada clase de evento de la
		jerarqu�a.
		*/
		virtual void execute() = 0;
		
	}; // class CEvent

} // namespace Logic

#endif // __Logic_Event_H
