//---------------------------------------------------------------------------
// EventManager.h
//---------------------------------------------------------------------------

/**
@file EventManager.h

Contiene la declaraci�n del gestor de eventos del juego. Los eventos ser�n
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author Ra�l Segura
@date Marzo, 2016
*/

#ifndef __Logic_EventManager_H
#define __Logic_EventManager_H

#include <queue>
#include <list>
#include <map>
#include <string>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class CEvent;
	enum ConditionEventType;
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
	Manager singleton que gestiona la carga y el lanzamiento de los
	eventos del juego.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Marzo, 2016
	*/
	class CEventManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static Logic::CEventManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos est�ticos.
		@return true si la inicializaci�n se hizo correctamente.
		*/
		static bool Init();

		/**
		Libera la propia instancia y los recursos est�ticos.
		Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Comprobar� la cola previamente cargada de eventos de tipo TIME
		a ver si alguno debe ser lanzado.

		@param msecs milisegundos transcurridos desde el �ltimo tick.
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
		Tipo �ndice de eventos condicionales del juego.
		*/
		typedef std::map<ConditionEventType, std::list<CEvent*>> TConditionEventMap;

		/**
		A�ade un evento a la cola de eventos lanzados por tiempo.

		@param event evento por tiempo a a�adir a la cola.
		@return true si se a�adi� correctamente.
		*/
		bool addTimeEvent(CEvent* ev);

		/**
		A�ade un evento al �ndice de eventos lanzados por condici�n.

		@param event evento condicional a a�adir al �ndice.
		@return true si se a�adi� correctamente.
		*/
		bool addConditionEvent(CEvent* ev);

		/**
		Lanza un evento condicional del tipo especificado.

		@param conditionEventType tipo de evento condicional a lanzar.
		@return true si se lanz� correctamente.
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
		Elimina todos los eventos lanzados por condici�n.
		*/
		void clearConditionEventsMap();

	protected:

		/**
		Cola de eventos lanzados por tiempo.
		*/
		TEventQueue _timeEvents;

		/**
		�ndice de eventos lanzados por condici�n.
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
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		est�ticas.

		@return true si la inicializaci�n se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberaci�n de los recursos
		est�ticos se hace en Release().
		*/
		void close();

	private:
		/**
		�nica instancia de la clase.
		*/
		static CEventManager *_instance;

	}; // class EventManager

} // namespace Logic

#endif // __Logic_EventManager_H
