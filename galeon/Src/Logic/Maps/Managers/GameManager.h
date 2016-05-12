//---------------------------------------------------------------------------
// GameManager.h
//---------------------------------------------------------------------------

/**
@file GameManager.h

Contiene la declaración del gestor del estado de la partida.

@author Álvaro Valera
@date Mayo, 2016
*/

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

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
*/
namespace Logic
{
	/**
	Manager singleton que gestiona el estado de la partida

	@ingroup logicGroup

	@author Álvaro Valera
	@date Mayo, 2016
	*/
	class CGameManager
	{
	public:
		/**
		Devuelve la única instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CGameManager *getSingletonPtr() { return _instance; }

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

		/** Método invocado cuando la ronda ha finalizado */
		void roundFinished();

		void tick(unsigned int msecs);

	protected:

		/**
		Constructor.
		*/
		CGameManager();

		/**
		Destructor.
		*/
		virtual ~CGameManager();

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
		static CGameManager *_instance;

	}; // class GameManager

} // namespace Logic

#endif // GAME_MANAGER_H_