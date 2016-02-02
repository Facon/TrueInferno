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

#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
		static Logic::CBuildingManager *getSingletonPtr() { return _instance; }

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
		Lista de edificios.
		Guardamos punteros de sus correspondientes componentes CPlaceable.
		*/
		std::vector<CPlaceable*> _buildings;

	}; // class BuildingManager

} // namespace Logic

#endif // BUILDING_MANAGER_H