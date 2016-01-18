//---------------------------------------------------------------------------
// TileManager.h
//---------------------------------------------------------------------------

/**
@file TileManager.h

Contiene la declaración del gestor de la matriz de tiles que da forma
al mapa.

@see Logic::CTileManager

@author Raúl Segura
@date Enero, 2016
*/

#ifndef __Logic_TileManager_H
#define __Logic_TileManager_H

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
	class CMap;
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
	Manager singleton que gestiona la lectura y actualización de la matriz
	de tiles del mapa.

	@ingroup logicGroup

	@author Raúl Segura
	@date Enero, 2016
	*/
	class CTileManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CTileManager *getSingletonPtr() { return _instance; }

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
		Lee y genera la matriz inicial de tiles del mapa.
		<p>
		Aquí el resto de entidades iniciales del mapa ya han sido cargadas
		de fichero (map.txt) y creadas. Entre ellas, habrá una Map::CEntity
		"Tile" que puede ser empleada de forma similar a un prefab para
		generar toda la matriz de tiles inicial.

		@param map Mapa en el que generar la matriz.
		*/
		void loadInitialMatrix(CMap *map);

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
		static CTileManager *_instance;

		/**
		Dimensiones de la matriz del mapa.
		*/
		const int SIZE_X = 15;
		const int SIZE_Z = 15;

	}; // class TileManager

} // namespace Logic

#endif // __Logic_TileManager_H