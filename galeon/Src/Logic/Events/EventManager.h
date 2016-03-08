//---------------------------------------------------------------------------
// EventManager.h
//---------------------------------------------------------------------------

/**
@file EventManager.h

Contiene la declaración del gestor de eventos del juego. Los eventos serán
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author Raúl Segura
@date Marzo, 2016
*/

#ifndef __Logic_EventManager_H
#define __Logic_EventManager_H

#include <queue>
#include <list>
#include <map>
#include <string>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
	class CEvent;
	enum ConditionEventType;
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
	Manager singleton que gestiona la carga y el lanzamiento de los
	eventos del juego.

	@ingroup logicGroup

	@author Raúl Segura
	@date Marzo, 2016
	*/
	class CEventManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static Logic::CEventManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos estáticos.
		@return true si la inicialización se hizo correctamente.
		*/
		static bool Init();

		/**
		Libera la propia instancia y los recursos estáticos.
		Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Comprobará la cola previamente cargada de eventos de tipo TIME
		a ver si alguno debe ser lanzado.

		@param msecs milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Carga el script de LUA encargado de leer y crear los eventos.

		@param filename nombre del script LUA con los eventos del juego.
		@return true si la carga se hizo correctamente.
		*/
		bool loadEvents(const std::string& filename);

		/**
		Destruye todos los eventos cargados.
		*/
		void unloadEvents();

		/**
		Tipo cola de eventos del juego.
		*/
		typedef std::queue<CEvent*> TEventQueue;

		/**
		Tipo índice de eventos condicionales del juego.
		*/
		typedef std::map<ConditionEventType, std::list<CEvent*>> TConditionEventMap;

		/**
		Añade un evento a la cola de eventos lanzados por tiempo.

		@param event evento por tiempo a añadir a la cola.
		@return true si se añadió correctamente.
		*/
		bool addTimeEvent(CEvent* ev);

		/**
		Añade un evento al índice de eventos lanzados por condición.

		@param event evento condicional a añadir al índice.
		@return true si se añadió correctamente.
		*/
		bool addConditionEvent(CEvent* ev);

		/**
		Lanza un evento condicional del tipo especificado.

		@param conditionEventType tipo de evento condicional a lanzar.
		@return true si se lanzó correctamente.
		*/
		bool launchConditionEvent(ConditionEventType conditionEventType);

		/**
		Devuelve la cola de eventos lanzados por tiempo del juego.
		@return cola de eventos por tiempo.
		*/
		const TEventQueue getTimeEventsQueue() { return _timeEvents; }

		/**
		Elimina todos los eventos lanzados por tiempo.
		*/
		void clearTimeEventsQueue();

		/**
		Elimina todos los eventos lanzados por condición.
		*/
		void clearConditionEventsMap();

	protected:

		/**
		Cola de eventos lanzados por tiempo.
		*/
		TEventQueue _timeEvents;

		/**
		Índice de eventos lanzados por condición.
		*/
		TConditionEventMap _conditionEvents;

		/**
		Constructor.
		*/
		CEventManager();

		/**
		Destructor.
		*/
		virtual ~CEventManager();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		estáticas.

		@return true si la inicialización se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberación de los recursos
		estáticos se hace en Release().
		*/
		void close();

	private:
		/**
		Única instancia de la clase.
		*/
		static CEventManager *_instance;

	}; // class EventManager

} // namespace Logic

#endif // __Logic_EventManager_H
