//---------------------------------------------------------------------------
// PowerManager.h
//---------------------------------------------------------------------------

/**
@file PowerManager.h

Contiene la declaraci�n del gestor de energ�a.

@see Logic::CPowerManager

@author �lvaro Valera
@date Marzo, 2016
*/

#ifndef POWER_MANAGER_H_
#define POWER_MANAGER_H_

#include "Logic/Maps/EntityID.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/Components/PowerGenerator.h"
#include "Logic/Entity/Components/PowerConsumer.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
	Manager singleton que proporciona funciones auxiliares a los componentes de energ�a.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Marzo, 2016
	*/

	class CPowerManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CPowerManager *getSingletonPtr() { return _instance; }

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
		Obtiene la entidad de PowerGenerator activa que pueda abastecer al PowerConsumer dado y tenga menor carga energ�tica.
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
		static CPowerManager *_instance;

		// Periodo (ms) con el que se chequean y se intentan abastecer los generadores con mayor necesidad
		int _refillPeriod;

		// Tiempo transcurrido desde el �ltimo reabastecimiento de los generadores
		int _timeSinceLastRefill;

		// Intenta reabastecer los generadores con mayor necesidad
		void refillPowerGenerators();

	}; // class PowerManager

} // namespace Logic

#endif // POWER_MANAGER_H_