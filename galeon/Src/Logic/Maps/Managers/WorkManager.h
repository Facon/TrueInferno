//---------------------------------------------------------------------------
// WorkManager.h
//---------------------------------------------------------------------------

/**
@file WorkManager.h

Contiene la declaración del gestor de trabajo.

@see Logic::CWorkManager

@author Álvaro Valera, Raúl Segura
@date Marzo, 2016
*/

#ifndef WORK_MANAGER_H_
#define WORK_MANAGER_H_

#include <utility>
#include <vector>

#include "Logic/Maps/EntityID.h"
#include "Logic/BuildingManager.h"
#include "Logic/Entity/Components/WorkBuilding.h"

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

	@author Álvaro Valera, Raúl Segura
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
		Método encargado de la comprobación y reordenación de los trabajadores
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
		Devuelve todos los edificios presentes en el mapa cuyo tipo pertenezca
		al grupo de prioridad dado.

		@param groupPriority prioridad del grupo de edificios a devolver.
		@return edificios pertenecientes a tipos pertenecientes al grupo.
		*/
		std::vector<CWorkBuilding*> getBuildingsFromGroup(int groupPriority);

		/**
		Dado un edificio concreto y la prioridad del grupo al que pertenece,
		intenta asignarle un cierto número de nuevos trabajadores procedentes
		de otros edificios de menor prioridad.

		@param building edificio para el que buscar nuevos trabajadores.
		@param groupPriority prioridad del grupo al que pertenece el edificio.
		@param lastPriorityGroup grupo de menor prioridad por el que empezar.
		@param numWorkers número de trabajadores necesarios.
		@return true si se encontró el número de trabajadores solicitado.
		*/
		bool relocateWorkers(CWorkBuilding* building, int groupPriority, int& lastPriorityGroup, int numWorkers);

	private:

		/**
		Única instancia de la clase.
		*/
		static CWorkManager *_instance;

		/**
		Lista que contiene todos los grupos de edificios ordenados por
		prioridad (definida por el jugador a través del HellQuarters).
		*/
		BuildingGroup _groupsPriority[NUM_BUILDING_GROUPS];

	}; // class WorkManager

} // namespace Logic

#endif // WORK_MANAGER_H_