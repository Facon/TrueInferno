//---------------------------------------------------------------------------
// SoulManager.h
//---------------------------------------------------------------------------

/**
@file SoulManager.h

Contiene la declaración del gestor de almas.

@see Logic::CSoulManager

@author Raúl Segura
@date Abril, 2016
*/

#ifndef __SOUL_MANAGER_H
#define __SOUL_MANAGER_H

#include <set>
#include <map>

#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/Soul.h"

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
	Manager singleton para centralizar la gestión de algunos aspectos
	del juego relativos a las almas.

	@ingroup logicGroup

	@author Raúl Segura
	@date Abril, 2016
	*/
	class CSoulManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CSoulManager *getSingletonPtr() { return _instance; }

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
		Registra un trabajador en el índice.
		Hay que llamar a unregisterWorker() cuando el trabajador vaya a
		destruirse o a changeWorkerBuilding() si va a cambiar de edificio.
		*/
		void registerWorker(CSoul *worker, CPlaceable *building);

		/**
		Elimina un trabajador del registro.
		*/
		void unregisterWorker(CSoul *worker);

		/**
		Asigna un trabajador previamente registrado a otro edificio.
		*/
		void changeWorkerBuilding(CSoul *worker, CPlaceable *newBuilding);

		/**
		Devuelve el conjunto de trabajadores de un edificio.
		*/
		std::set<CSoul*>* getBuildingWorkers(CPlaceable *building);

		/**
		Devuelve la velocidad de movimiento de las almas.
		*/
		float getMovementSpeed() const { return _movementSpeed; }

		/**
		Modifica la velocidad de movimiento de las almas.
		*/
		void setMovementSpeed(float movementSped)
		{
			_movementSpeed = movementSped;
			_realMovementSpeed = movementSped;
		}

		/**
		Métodos encargados de gestionar modificaciones temporales en la
		velocidad de movimiento de las almas.
		*/
		void modifyMovementSpeed(float factor);
		void restoreMovementSpeed();

	protected:

		/**
		Constructor.
		*/
		CSoulManager();

		/**
		Destructor.
		*/
		virtual ~CSoulManager();

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

		/**
		Velocidad de movimiento de las almas.
		*/
		float _movementSpeed = 1.5f;
		float _realMovementSpeed = _movementSpeed;

	private:

		/**
		Única instancia de la clase.
		*/
		static CSoulManager *_instance;

		/**
		Índice que contiene la referencia a cada uno de los trabajadores
		del juego agrupados por el edificio en el que trabajan.
		Se almacenan punteros a sus correspondientes componentes CSoul.
		*/
		std::map<CPlaceable*, std::set<CSoul*>*> _workers;

	}; // class CSoulManager

} // namespace Logic

#endif // __SOUL_MANAGER_H