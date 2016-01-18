//---------------------------------------------------------------------------
// EmptyManagerTemplate.h
//---------------------------------------------------------------------------

/**
THIS IS A TEMPLATE OF AN EMPTY SINGLETON MANAGER, CONTAINING ONLY THE
MANAGER INSTANCE AND ALL REQUIRED FUNCTIONS FOR INITIALIZATION AND
DESTRUCTION OF STATIC RESOURCES.

TO CREATE A NEW MANAGER FROM THIS, SIMPLY:
1) COPY-PASTE THIS CODE.
2) CLOSE COMMENTS.
3) CHANGE #ifndef, #define, THE CLASS NAME AND EVERY OTHER THING YOU NEED.
4) REMEMBER ALSO TO REVIEW AND CHANGE COMMENTS IF NEEDED.
*/

/**
@file TileManager.h

Contiene la declaración del gestor de la matriz de tiles que da forma
al mapa.

@see Logic::CTileManager

@author Raúl Segura
@date Enero, 2016
/

#ifndef __Logic_TileManager_H
#define __Logic_TileManager_H

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic
{
}

/**
Namespace que engloba la lógica del juego. Engloba desde el mapa lógico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para más información ver @ref LogicGroup).

@author David Llansó
@date Agosto, 2010
/
namespace Logic
{
	/**
	Manager singleton que gestiona la lectura y actualización de la matriz
	de tiles del mapa.

	@ingroup logicGroup

	@author Raúl Segura
	@date Enero, 2016
	/
	class CTileManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		/
		static Logic::CTileManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos estáticos.

		@return true si la inicialización se hizo correctamente.
		/
		static bool Init();

		/**
		Libera la propia instancia y los recursos estáticos.
		Debe llamarse al finalizar la aplicación.
		/
		static void Release();

	protected:

		/**
		Constructor.
		/
		CTileManager();

		/**
		Destructor.
		/
		virtual ~CTileManager();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		estáticas.

		@return true si la inicialización se hizo correctamente.
		/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberación de los recursos
		estáticos se hace en Release().
		/
		void close();

	private:
		/**
		Única instancia de la clase.
		/
		static CTileManager *_instance;

	}; // class TileManager

} // namespace Logic

#endif // __Logic_TileManager_H

*/