//---------------------------------------------------------------------------
// BuildingManager.h
//---------------------------------------------------------------------------

/**
@file BuildingManager.h

Contiene la declaración del gestor de edificios.

@see Logic::CBuildingManager

@author Álvaro Valera
@date Enero, 2016
*/

#ifndef BUILDING_MANAGER_H
#define BUILDING_MANAGER_H

#include <vector>
#include <string>
#include <map>
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/Message.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
	class CMap;
	class CPlaceable;
	class CEntity;
}

namespace Map
{
	class CEntity;
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
	Manager singleton que gestiona las operaciones con los edificios del mapa.

	@ingroup logicGroup

	@author Álvaro Valera
	@date Enero, 2016
	*/
	class CBuildingManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static CBuildingManager *getSingletonPtr() { return _instance; }

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
		Lee y genera la lista inicial de edificios del mapa.
		<p>
		Aquí el resto de entidades iniciales del mapa ya han sido cargadas
		de fichero (map.txt) y creadas. Entre ellas, habrá diferentes Map::CEntity,
		cada una con un tipo específico, que servirán como "prefab" para instanciar
		los diferentes tipos de edificio.

		@param map Mapa en el que generar los edificios.
		*/
		bool loadInitialBuildings(CMap *map);

		/**
		Registra internamente un edificio ya existente.
		Hay que llamar a unregisterBuilding cuando el edificio se quiera destruír. Actualmente esto se hace en el destructor de CPlaceable
		*/
		void registerBuilding(CPlaceable *placeable);

		/**
		Saca del registro un edificio previamente registrado.
		*/
		void unregisterBuilding(CPlaceable *placeable);

		/** Coloca un placeable del tipo indicado con el tile inicial ubicado en la posición lógica dada

		@param map Mapa donde se instanciará la entidad del edificio
		@param prefabName Nombre de la entidad de mapa o "prefab"
		@param logicPosition Posición lógica donde se colocará el primer tile. El resto de tiles se colocarán de acuerdo a la info del prefab
		@param floating Flag a true si se quiere dejar al Placeable inicialmente flotando sin ocupar realmente posiciones en la matriz ni registrarse */
		void CBuildingManager::destroyPlaceable(CEntity *entity);
		CEntity* createPlaceable(CMap *map, const std::string& prefabName, const Vector3& logicPosition, bool floating, bool showFloating);

		/** Hace flotar un placeable hasta la posición lógica deseada. Es necesario hacer llamar a place para que deje de flotar, se registre y conste para los chequeos de posicionamiento */
		bool floatPlaceableTo(CEntity* movableEntity, const Vector3& logicPosition, bool showFloating);

		/** Coloca un placeable en la posición actual donde está. Deja de flotar, se registra y comienza a constar para los chequeos de posicionamiento */
		bool placePlaceable(CEntity* movableEntity);

		/** Comprueba si un placeable está en una posición válida */
		bool checkValidPlaceablePosition(CEntity* placeableEntity, const Vector3& logicPosition);

		// Localiza un edificio por el tipo dado
		CPlaceable* findBuilding(BuildingType buildingType);

		// Devuelve un edificio aleatorio
		CPlaceable* getRandomBuilding();

		//Destruye un edificio aleatorio
		bool DestroyRandomBuilding();

		/** Transmite un mensaje a todos los edificios registrados. Devuelve true si alguno aceptó el mensaje */
		bool HandleMessage(const LogisticsMessage& msg);

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
		static CBuildingManager *_instance;

		/**
		Estructura map para almacenar los edificios. La clave es el tipo de edificio, el valor es un puntero al set de todos los edificios de dicho tipo.
		Guardamos punteros de sus correspondientes componentes CPlaceable.
		*/
		std::map<BuildingType, std::set<CPlaceable*>*> _buildings;

		/**
		Índice de prefabs de edificios indexados por su nombre de tipo
		*/
		std::map<std::string, Map::CEntity*> _prefabs;

		/** Carga los prefabs de edificios del mapa y los almacena indexados internamente */
		//bool loadPrefabs();

		void printBuildingList() const;

		CPlaceable* getRandomBuildingforDestruction();
		bool CBuildingManager::checkValidBuildingTypeforDestruction();

	}; // class BuildingManager

} // namespace Logic

#endif // BUILDING_MANAGER_H