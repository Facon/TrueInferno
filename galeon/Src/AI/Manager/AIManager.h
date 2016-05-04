//---------------------------------------------------------------------------
// AIManager.h
//---------------------------------------------------------------------------

/**
@file AIManager.h

Contiene la declaración del gestor de la IA de los enemigos en el juego. 

@see Logic::CScriptManager

@author Álvaro Valera
@date Abril, 2016
*/

#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include <list>
#include <vector>
#include <map>
#include <string>

#include "Logic/Events/Event.h"
#include "Logic/Entity/MessageHandler.h"

// Predeclaración de clases para ahorrar tiempo de compilación
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
	Manager singleton que gestiona la IA de los enemigos en el juego.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Abril, 2016
	*/
	class CAIManager : public MessageHandler
	{
	public:

		/**
		Devuelve la única instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CAIManager *getSingletonPtr() { return _instance; }

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
		Si ha pasado determinado tiempo desde el último evento, lanzará un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Carga un script de LUA encargado de crear y lanzar eventos.

		@param filename nombre del script.
		@return true si la carga se hizo correctamente.
		*/
		bool loadAIScript(const std::string& filename);

		/**
		Devuelve el tiempo (ms) de la aplicación.
		Es una función-puente para que Lua no tenga que acceder a otras partes del motor.

		@return Tiempo (ms) que la aplicación lleva activa.
		*/
		long getGlobalTime() const;

		// Manejo de mensajes, tiene que manejar todos los tipos de mensajes sin excepción.
		bool HandleMessage(const Message& msg);
		bool HandleMessage(const TransformMessage& msg);
		bool HandleMessage(const PositionMessage& msg);
		bool HandleMessage(const RotationMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& msg);
		bool HandleMessage(const MaterialMessage& msg);
		bool HandleMessage(const AnimationMessage& msg);
		bool HandleMessage(const ControlMessage& msg);
		bool HandleMessage(const PhysicMessage& msg);
		bool HandleMessage(const TouchMessage& msg);
		bool HandleMessage(const DamageMessage& msg);
		bool HandleMessage(const WorkerMessage& msg);
		bool HandleMessage(const WalkSoulPathMessage& msg);
		bool HandleMessage(const HellQuartersMessage& msg);
		bool HandleMessage(const MovePlaceableMessage& msg);
		bool HandleMessage(const SoulSenderMessage& msg);
		bool HandleMessage(const SoulMessage& msg);
		bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);
		bool HandleMessage(const NumberMessage& msg);
		bool HandleMessage(const ResourceMessage& msg);
		bool HandleMessage(const GetCostPlaceableMessage& msg);
		bool HandleMessage(const LogisticsMessage& msg);
		bool HandleMessage(const ToggleMessage& msg);

	protected:

		/**
		Constructor.
		*/
		CAIManager();

		/**
		Destructor.
		*/
		virtual ~CAIManager();

		/**
		Método encargado de registrar en el contexto de Lua todas aquellas
		clases y funciones necesarias para el completo manejo de los eventos
		de juego desde Lua.
		*/
		void luaRegister();

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
		static CAIManager *_instance;

	}; // class AIManager

} // namespace Logic

#endif // AI_MANAGER_H_
