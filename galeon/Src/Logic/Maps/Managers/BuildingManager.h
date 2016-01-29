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

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
	class CMap;
	class CPlaceable;
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
		static Logic::CBuildingManager *getSingletonPtr() { return _instance; }

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
		void loadInitialBuildings(CMap *map);

		/**
		Registra internamente un edificio.
		*/
		void registerBuilding(CPlaceable *placeable);

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
		Lista de edificios.
		Guardamos punteros de sus correspondientes componentes CPlaceable.
		*/
		std::vector<CPlaceable*> _buildings;

	}; // class BuildingManager

} // namespace Logic

#endif // BUILDING_MANAGER_H