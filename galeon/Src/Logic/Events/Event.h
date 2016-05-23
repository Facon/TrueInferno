//---------------------------------------------------------------------------
// Event.h
//---------------------------------------------------------------------------

/**
@file Event.h

Evento de juego. Los eventos se dividirán de dos formas posibles: en función
de su tipo, como eventos de información y eventos de decisión, y en función
de su trigger, como eventos lanzados por tiempo y por condición/acción.

@see Logic::CEventManager

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_Event_H
#define __Logic_Event_H

#include <string>

#include "Logic/TimeManager.h"

// Predeclaración de clases para ahorrar tiempo de compilación.
namespace Logic
{
	enum ConditionTriggerType;
}

namespace AI
{
	class CGod;
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
			// Eventos de decisión que obligarán al jugador a elegir
			// entre 2 o más opciones, provocando cada una de ellas
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
			// condición durante la partida.
			CONDITION
		};

		/**
		Posibles tipos de triggers CONDITION.
		IMPORTANTE: No cambiar el valor entero asociado a cada valor del
		enumerado y añadir cada nuevo valor en CEvent::luaRegister().
		*/
		enum ConditionTriggerType
		{
			// Eventos disparados como parte del tutorial para guiar al
			// jugador durante los primeros instantes de la partida.
			TUTORIAL,
			// Eventos disparados al final de la partida para mostrar el
			// resultado de la misma y la puntuación final.
			END_GAME,
			// Eventos disparados al final de la ronda para mostrar el
			// resultado de la misma.
			END_ROUND
		};

		/**
		Constructores.
		*/

		/**
		Construye un evento con trigger basado en tiempo.

		@param type Tipo del evento.
		@param time En función del parámetro absoluteTime, instante temporal absoluto en el que se lanzará el evento
		o relativo al instante actual.
		@absoluteTime Flag a true para lanzar el evento en un instante temporal absoluto o relativo al instante actual.
		@showImmediately Flag a true si el evento debe mostrarse inmediatamente en vez de notificarse primero
		TODO ...
		*/
		CEvent(EventType type, unsigned long time, bool absoluteTime, bool showImmediately, const std::string& godName);

		/**
		Construye un evento con trigger basado en condición.

		TODO ...
		*/
		CEvent(EventType type, ConditionTriggerType conditionType, bool showImmediately, const std::string& godName);

		/**
		Destructor.
		*/
		virtual ~CEvent() {}

		/**
		Registra esta clase evento en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
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

		@param[out] keepAlive true si el evento debe mantenerse vivo o 
		puede ser borrado tras el lanzamiento.

		@return true si el evento fue lanzado.
		*/
		bool launch(bool& keepAlive);

		int getEventId() const {
			return _eventId;
		}

		/** Devuelve la imagen para la GUI */
		virtual std::string getGUIImageName() const = 0;

		/** Devuelve el título para la GUI */
		virtual std::string getGUITitle() const = 0;

		/** Devuelve el campo de texto para la GUI */
		virtual std::string getGUIText() const = 0;

		/** Devuelve el campo adicional de texto para la GUI */
		virtual std::string getGUIResumeText() const = 0;

		/** Devuelve si el evento debe ser notificado */
		bool mustBeNotified() const;

		bool getShowImmediately() const { return _showImmediately; }

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
		Tipo de condición que lanza el evento.
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
		Método abstracto que ejecuta la lógica específica asociada al
		evento. Deberá ser sobreescrito por cada clase de evento de la
		jerarquía.
		*/
		virtual void execute() = 0;

		/** Id interno del evento */
		int _eventId;

		/** Contador estático del id de evento */
		static int _nextEventId;

		/** Dios que provoca el evento (si es que hay alguno) */
		AI::CGod* _god;

		/** Título para la GUI */
		std::string _guiTitle;

		/** Imagen para la GUI */
		std::string _guiImageName;

		/** Campo de texto para la GUI */
		std::string _guiText;

		/** Campo adicional de texto para la GUI */
		std::string _guiResumeText;

		/** Dios que provoca el evento */
		AI::CGod *god;

		/** Flag a true si el evento debe mostrarse inmediatamente en vez de notificarse primero */
		bool _showImmediately;

	}; // class CEvent

} // namespace Logic

#endif // __Logic_Event_H
