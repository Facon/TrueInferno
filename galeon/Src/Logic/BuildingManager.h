//---------------------------------------------------------------------------
// BuildingManager.h
//---------------------------------------------------------------------------

/**
@file BuildingManager.h

Contiene la declaraci�n del gestor de edificios.

@see Logic::CBuildingManager

@author �lvaro Valera
@date Enero, 2016
*/

#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#define NUM_BUILDING_GROUPS 6

#include <vector>
#include <string>
#include <map>
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/Message.h"
#include "Logic/Entity/Components/Placeable.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class CMap;
	class CEntity;
}

namespace Map
{
	class CEntity;
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
	Grupos de edificios por tipo.
	�til para asignaci�n de trabajadores por prioridad.
	CUIDAO!: En caso de modificaci�n, cambiar tambi�n NUM_BUILDING_GROUPS
	         y el WorkManager.
	*/
	enum BuildingGroup {
		PowerSupply,    // PowerGenerator
		MapResources,   // Mine, GasPlant
		Production,     // Furnace
		EvilProcessing, // Evilworks, Refinery
		Logistics,      // Warehouse, Evilator
		IDi,            // ResearchLab
		//Bureaucracy   // HellQuarters
		Null
		/*
		De momento no es necesario a�adir el HellQuarters, ya que no
		tendr� trabajadores como tal, sino que consideraremos que
		funciona siempre (con personal del propio Infierno :)).
		*/
	};

	/**
	Manager singleton que gestiona las operaciones con los edificios del mapa.

	@ingroup logicGroup

	@author �lvaro Valera
	@date Enero, 2016
	*/
	class CBuildingManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static CBuildingManager *getSingletonPtr() { return _instance; }

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
		Lee y genera la lista inicial de edificios del mapa.
		<p>
		Aqu� el resto de entidades iniciales del mapa ya han sido cargadas
		de fichero (map.txt) y creadas. Entre ellas, habr� diferentes Map::CEntity,
		cada una con un tipo espec�fico, que servir�n como "prefab" para instanciar
		los diferentes tipos de edificio.

		@param map Mapa en el que generar los edificios.
		*/
		bool loadInitialBuildings(CMap *map);

		/**
		Registra internamente un edificio ya existente.
		Hay que llamar a unregisterBuilding cuando el edificio se quiera destru�r. Actualmente esto se hace en el destructor de CPlaceable
		*/
		void registerBuilding(CPlaceable *placeable);

		/**
		Saca del registro un edificio previamente registrado.
		*/
		void unregisterBuilding(CPlaceable *placeable);

		/** Coloca un placeable del tipo indicado con el tile inicial ubicado en la posici�n l�gica dada

		@param map Mapa donde se instanciar� la entidad del edificio
		@param prefabName Nombre de la entidad de mapa o "prefab"
		@param logicPosition Posici�n l�gica donde se colocar� el primer tile. El resto de tiles se colocar�n de acuerdo a la info del prefab
		@param floating Flag a true si se quiere dejar al Placeable inicialmente flotando sin ocupar realmente posiciones en la matriz ni registrarse */
		CEntity* createPlaceable(CMap *map, const std::string& prefabName, const Vector3& logicPosition, bool floating, bool showFloating);

		/** Elimina el Placeable dado incluyendo todas las operaciones de mantenimiento necesarias como, por ejemplo, desregistrar el edificio de las listas internas */
		void CBuildingManager::destroyPlaceable(CEntity *entity);

		/** Hace flotar un placeable hasta la posici�n l�gica deseada. Es necesario hacer llamar a place para que deje de flotar, se registre y conste para los chequeos de posicionamiento */
		bool floatPlaceableTo(CEntity* movableEntity, const Vector3& logicPosition, bool showFloating);

		/** Coloca un placeable en la posici�n actual donde est�. Deja de flotar, se registra y comienza a constar para los chequeos de posicionamiento */
		bool placePlaceable(CEntity* movableEntity);

		/** Comprueba si un placeable est� en una posici�n v�lida */
		bool checkValidPlaceablePosition(CEntity* placeableEntity, const Vector3& logicPosition);

		// Devuelve el n�mero de edificios de un tipo dado
		int getBuildingTypeNumber(BuildingType buildingType);

		// Localiza un edificio por el tipo dado
		CPlaceable* findBuilding(BuildingType buildingType);

		// Devuelve un edificio aleatorio
		CPlaceable* getRandomBuilding();

		//Destruye un edificio aleatorio
		bool DestroyRandomBuilding();

		/** Devuelve la estructura de datos con los edificios */
		std::map<BuildingType, std::set<CPlaceable*>*>& getBuildings();

		/** Transmite un mensaje a todos los edificios registrados. Devuelve true si alguno acept� el mensaje */
		bool HandleMessage(const LogisticsMessage& msg);

		/** Transmite un mensaje a todos los edificios registrados. Devuelve true si alguno acept� el mensaje */
		bool HandleMessage(const ResourceMessage& msg);

		template <class ComponentClass>
		std::vector<ComponentClass*> getBuildings(){
			std::vector<ComponentClass*> list;

			// Para cada tipo de edificio registrado
			for (auto itType = _buildings.cbegin(); itType != _buildings.cend(); ++itType){
				if ((itType->second) == nullptr)
					continue;

				// Para cada uno de sus edificios
				for (auto itBuilding = itType->second->cbegin(); itBuilding != itType->second->cend(); ++itBuilding){
					ComponentClass *component = (*itBuilding)->getEntity()->getComponent<ComponentClass>();
					if (component != nullptr)
						list.push_back(component);
				}
			}

			return list;
		}

	protected:

		/**
		Constructor.
		*/
		CBuildingManager();

		/**
		Destructor.
		*/
		virtual ~CBuildingManager();

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
		static CBuildingManager *_instance;

		/**
		Estructura map para almacenar los grupos de edificios por tipo.
		*/
		std::map<BuildingGroup, std::set<BuildingType>*> _buildingGroups;

		/**
		Estructura map para almacenar los edificios. La clave es el tipo de edificio,
		el valor es un puntero al set de todos los edificios de dicho tipo.
		Guardamos punteros de sus correspondientes componentes CPlaceable.
		*/
		std::map<BuildingType, std::set<CPlaceable*>*> _buildings;

		/**
		�ndice de prefabs de edificios indexados por su nombre de tipo
		*/
		std::map<std::string, Map::CEntity*> _prefabs;

		void printBuildingList() const;

		CPlaceable* getRandomBuildingforDestruction();
		bool CBuildingManager::checkValidBuildingTypeforDestruction();

	}; // class BuildingManager

} // namespace Logic

#endif // BUILDING_MANAGER_H