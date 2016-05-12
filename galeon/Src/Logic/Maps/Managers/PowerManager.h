//---------------------------------------------------------------------------
// PowerManager.h
//---------------------------------------------------------------------------

/**
@file PowerManager.h

Contiene la declaración del gestor de energía.

@see Logic::CPowerManager

@author Álvaro Valera
@date Marzo, 2016
*/

#ifndef POWER_MANAGER_H_
#define POWER_MANAGER_H_

#include "Logic/Maps/EntityID.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/Components/PowerGenerator.h"
#include "Logic/Entity/Components/PowerConsumer.h"

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
	Manager singleton que proporciona funciones auxiliares a los componentes de energía.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Marzo, 2016
	*/

	class CPowerManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CPowerManager *getSingletonPtr() { return _instance; }

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
		Obtiene la entidad de PowerGenerator activa que pueda abastecer al PowerConsumer dado y tenga menor carga energética.
		*/
		TEntityID findPowerGenerator(CEntity* powerConsumer);

		void tick(unsigned int msecs);

	protected:

		/**
		Constructor.
		*/
		CPowerManager();

		/**
		Destructor.
		*/
		virtual ~CPowerManager();

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
		static CPowerManager *_instance;

		// Periodo (ms) con el que se chequean y se intentan abastecer los generadores con mayor necesidad
		int _refillPeriod;

		// Tiempo transcurrido desde el último reabastecimiento de los generadores
		int _timeSinceLastRefill;

		// Intenta reabastecer los generadores con mayor necesidad
		void refillPowerGenerators();

	}; // class PowerManager

} // namespace Logic

#endif // POWER_MANAGER_H_