//---------------------------------------------------------------------------
// TileManager.h
//---------------------------------------------------------------------------

/**
@file TileManager.h

Contiene la declaraci�n del gestor de la matriz de tiles que da forma
al mapa.

@see Logic::CTileManager

@author Ra�l Segura
@date Enero, 2016
*/

#ifndef __Logic_TileManager_H
#define __Logic_TileManager_H

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
	class CMap;
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
		const int SIZE_X = 15;
		const int SIZE_Z = 15;

	}; // class TileManager

} // namespace Logic

#endif // __Logic_TileManager_H