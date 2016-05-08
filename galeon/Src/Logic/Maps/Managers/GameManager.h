//---------------------------------------------------------------------------
// GameManager.h
//---------------------------------------------------------------------------

/**
@file GameManager.h

Contiene la declaraci�n del gestor del estado de la partida.

@author �lvaro Valera
@date Mayo, 2016
*/

#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic
{
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
	Manager singleton que gestiona el estado de la partida

	@ingroup logicGroup

	@author �lvaro Valera
	@date Mayo, 2016
	*/
	class CGameManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero a la instancia de la clase.
		*/
		static Logic::CGameManager *getSingletonPtr() { return _instance; }

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

		/** M�todo invocado cuando la ronda ha finalizado */
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
		static CGameManager *_instance;

	}; // class GameManager

} // namespace Logic

#endif // GAME_MANAGER_H_