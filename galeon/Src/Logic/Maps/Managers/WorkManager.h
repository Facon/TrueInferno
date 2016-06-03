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

#include <utility>

#include "Logic/Maps/EntityID.h"
#include "Logic/BuildingManager.h"

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

		/**
		Devuelve la lista de grupos de edificios ordenados por prioridad.
		*/
		BuildingGroup* getGroupsPriority() { return _groupsPriority; }

		/**
		Define la nueva prioridad de cada grupo de edificios a partir del valor
		asignado a cada uno por el jugador.

		@return 0 si todo fue correcto. En caso contrario, un valor entre 1 y
		NUM_BUILDING_GROUPS correspondiente al orden de prioridad no encontrado.
		*/
		unsigned int setGroupsPriority(std::pair<BuildingGroup, unsigned int> newGroupsPriority[NUM_BUILDING_GROUPS]);

		/**
		M�todo encargado de la comprobaci�n y reordenaci�n de los trabajadores
		(en caso de que sea necesario) en base a la prioridad definida para
		cada grupo de edificios.
		*/
		void reassignWorkers();

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

		/**
		Lista que contiene todos los grupos de edificios ordenados por
		prioridad (definida por el jugador a trav�s del HellQuarters).
		*/
		BuildingGroup _groupsPriority[NUM_BUILDING_GROUPS];

	}; // class WorkManager

} // namespace Logic

#endif // WORK_MANAGER_H_