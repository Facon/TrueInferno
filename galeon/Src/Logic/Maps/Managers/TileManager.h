//---------------------------------------------------------------------------
// TileManager.h
//---------------------------------------------------------------------------

/**
@file TileManager.h

Contiene la declaraci�n del gestor de la matriz de tiles que da forma
al mapa.

@see Logic::CTileManager

@author Ra�l Segura
@author �lvaro Valera
@date Enero, 2016
*/

#ifndef __Logic_TileManager_H
#define __Logic_TileManager_H

#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class CMap;
	class CEntity;
	class Tile;
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
	Manager singleton que gestiona la lectura y actualizaci�n de la matriz
	de tiles del mapa.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Enero, 2016
	*/
	class CTileManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CTileManager *getSingletonPtr() { return _instance; }

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
		Lee y genera la matriz inicial de tiles del mapa.
		<p>
		Aqu� el resto de entidades iniciales del mapa ya han sido cargadas
		de fichero (map.txt) y creadas. Entre ellas, habr� una Map::CEntity
		"Tile" que puede ser empleada de forma similar a un prefab para
		generar toda la matriz de tiles inicial.

		@param map Mapa en el que generar la matriz.
		*/
		bool loadInitialMatrix(CMap *map);

		/**
		Register internally a tile in the matrix tile.
		*/
		void registerTile(Tile *tile);

		/**
		Returns a tile given its logic position or null if coordinates are out of bounds
		*/
		Tile* getTile(const Vector3 &position);

		Tile* getTileByName(std::string tileName);
			
		/** Devuelve un puntero a una tile aleatoria */
		Tile* getRandomTile();

		/** Devuelve un puntero a la Tile m�s cercana a la posici�n real dada */
		Tile* getNearestTile(const Vector3 &pos) const;

		/** Imprime informaci�n de debug */
		void printDebugInfo();

		/** Devuelve las dimensiones de la matriz de tiles */
		void getDimensions(int& x, int& z) { x = SIZE_X; z = SIZE_Z; }

	protected:

		/**
		Constructor.
		*/
		CTileManager();

		/**
		Destructor.
		*/
		virtual ~CTileManager();

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
		static CTileManager *_instance;

		/**
		Dimensiones de la matriz del mapa.
		*/
		static const int SIZE_X;
		static const int SIZE_Z;

		/**
		File with the terrain map definition.
		*/
		static const std::string TERRAIN_MAP_FILE;

		/**
		2D tile matrix[x][z].
		We store pointers to their Tile components.
		*/
		Tile* **_tiles;

		/**
		Entidad de mapa "Tile" le�da del fichero de mapa a modo de prefab.
		*/
		Map::CEntity *tileMapEntity;

		/**
		Assign terrain type to all tiles in the map reading them from the given file
		- 0 stands for Empty
		- 1 stands for Gas
		- 2 stands for Mineral

		El fichero se lee de acuerdo a los ejes gr�ficos de Gale�n-Ogre con el origen de coordenadas en la esquina inferior derecha:
		(X-1, Z-1) (X-2, Z-1) ... (1, Z-1) (0, Z-1)
		(X-1, Z-2) (X-2, Z-2) ... (1, Z-2) (0, Z-2)
		...
		(X-1, 1) (X-2, 1) ... (1, 1) (0, 1)
		(X-1, 0) (X-2, 0) ... (1, 0) (0, 0)
		*/
		void loadTerrain(const std::string &filename);

	}; // class TileManager

} // namespace Logic

#endif // __Logic_TileManager_H