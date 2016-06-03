//---------------------------------------------------------------------------
// SoulManager.h
//---------------------------------------------------------------------------

/**
@file SoulManager.h

Contiene la declaraci�n del gestor de almas.

@see Logic::CSoulManager

@author Ra�l Segura
@date Abril, 2016
*/

#ifndef __SOUL_MANAGER_H
#define __SOUL_MANAGER_H

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
	Manager singleton para centralizar la gesti�n de algunos aspectos
	del juego relativos a las almas.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Abril, 2016
	*/
	class CSoulManager
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CSoulManager *getSingletonPtr() { return _instance; }

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
		Devuelve la velocidad de movimiento de las almas.
		*/
		float getMovementSpeed() const { return _movementSpeed; }

		/**
		Modifica la velocidad de movimiento de las almas.
		*/
		void setMovementSpeed(float movementSped)
		{
			_movementSpeed = movementSped;
			_realMovementSpeed = movementSped;
		}

		/**
		M�todos encargados de gestionar modificaciones temporales en la
		velocidad de movimiento de las almas.
		*/
		void modifyMovementSpeed(float factor);
		void restoreMovementSpeed();

	protected:

		/**
		Constructor.
		*/
		CSoulManager();

		/**
		Destructor.
		*/
		virtual ~CSoulManager();

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

		/**
		Velocidad de movimiento de las almas.
		*/
		float _movementSpeed = 1.5f;
		float _realMovementSpeed = _movementSpeed;

	private:

		/**
		�nica instancia de la clase.
		*/
		static CSoulManager *_instance;

	}; // class CSoulManager

} // namespace Logic

#endif // __SOUL_MANAGER_H