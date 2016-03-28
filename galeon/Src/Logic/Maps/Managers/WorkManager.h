//---------------------------------------------------------------------------
// WorkManager.h
//---------------------------------------------------------------------------

/**
@file WorkManager.h

Contiene la declaración del gestor de trabajo.

@see Logic::CWorkManager

@author Álvaro Valera
@date Marzo, 2016
*/

#ifndef WORK_MANAGER_H_
#define WORK_MANAGER_H_

#include "Logic/Maps/EntityID.h"
#include "Logic/Entity/BuildingType.h"

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
	Manager singleton que gestiona la asignación de tareas a los trabajadores.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Marzo, 2016
	*/

	class CWorkManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CWorkManager *getSingletonPtr() { return _instance; }

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
		Obtiene la entidad del edificio donde sería más conveniente añadir trabajadores.
		Si se indica un tipo de edificio sólo se buscará entre edificios de dicho tipo.
		*/
		TEntityID findBuildingToWork(BuildingType buildingType = BuildingType::Unassigned);

		/**
		Obtiene la entidad de horno activo con más capacidad actual para quemar almas.
		*/
		TEntityID findFurnace();

	protected:

		/**
		Constructor.
		*/
		CWorkManager();

		/**
		Destructor.
		*/
		virtual ~CWorkManager();

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
		static CWorkManager *_instance;

	}; // class WorkManager

} // namespace Logic

#endif // WORK_MANAGER_H_