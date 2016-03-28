//---------------------------------------------------------------------------
// WorkManager.h
//---------------------------------------------------------------------------

/**
@file WorkManager.h

Contiene la declaraci�n del gestor de trabajo.

@see Logic::CWorkManager

@author �lvaro Valera
@date Marzo, 2016
*/

#ifndef WORK_MANAGER_H_
#define WORK_MANAGER_H_

#include "Logic/Maps/EntityID.h"
#include "Logic/Entity/BuildingType.h"

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
	Manager singleton que gestiona la asignaci�n de tareas a los trabajadores.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Marzo, 2016
	*/

	class CWorkManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CWorkManager *getSingletonPtr() { return _instance; }

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
		Obtiene la entidad del edificio donde ser�a m�s conveniente a�adir trabajadores.
		Si se indica un tipo de edificio s�lo se buscar� entre edificios de dicho tipo.
		*/
		TEntityID findBuildingToWork(BuildingType buildingType = BuildingType::Unassigned);

		/**
		Obtiene la entidad de horno activo con m�s capacidad actual para quemar almas.
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
		static CWorkManager *_instance;

	}; // class WorkManager

} // namespace Logic

#endif // WORK_MANAGER_H_