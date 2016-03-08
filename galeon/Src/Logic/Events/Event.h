//---------------------------------------------------------------------------
// Event.h
//---------------------------------------------------------------------------

/**
@file Event.h

Evento de juego. Los eventos se dividirán de dos formas posibles: en función
de su tipo, como eventos de información y eventos de decisión, y en función
de su trigger, como eventos lanzados por tiempo y por condición/acción.

@see Logic::Event

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_Event_H
#define __Logic_Event_H

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
	Clase abstracta raíz de la jerarquía de eventos del juego.

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CEvent
	{

	public:

		enum EventType
		{
			// Eventos habituales del juego. Normalmente informativos,
			// ya sea sobre acciones pasadas, para mantener al jugador
			// informado sobre sucesos importantes, o futuras, a las
			// que pueda o deba reaccionar de una forma u otra.
			INFO,
			// Eventos de decisión que obligarán al jugador a elegir
			// entre 2 o más opciones, provocando cada una de ellas
			// un resultado diferente sobre la partida.
			DECISION
		};

		enum EventTrigger
		{
			// Eventos lanzados al llegar a un tiempo concreto de juego.
			TIME,
			// Eventos lanzados cuando se cumple una determinada
			// condición durante la partida.
			// @TODO Implementar toda la lógica de este tipo de eventos.
			CONDITION
		};

		/**
		Constructores.
		*/
		CEvent(EventType type, unsigned int time) :
			_type(type), _trigger(TIME), _time(time) {};

		/**
		Destructor.
		*/
		virtual ~CEvent() {}

		/**
		Getters.
		*/
		EventType getEventType() { return _type; }
		EventTrigger getEventTrigger() { return _trigger; }

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
		Tiempo de lanzamiento del evento en milisegundos.
		NULL si _trigger != TIME.
		*/
		unsigned int _time;

		/**
		Comprueba si el evento debe ser lanzado atendiendo a su trigger.
		*/
		bool mustBeLaunched();

		/**
		Método abstracto que ejecuta la lógica específica asociada al
		evento. Deberá ser sobreescrito por cada clase de evento de la
		jerarquía.
		*/
		virtual void execute() = 0;
		
	}; // class CEvent

} // namespace Logic

#endif // __Logic_Event_H
