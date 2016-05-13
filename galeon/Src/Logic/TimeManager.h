#ifndef TIME_MANAGER_H_
#define TIME_MANAGER_H_

namespace Logic
{
	class CTimeManager
	{

	public:
		static CTimeManager& getSingleton()
		{ return _instance; }

		static CTimeManager* getSingletonPtr()
		{ return &_instance; }

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
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Si ha pasado determinado tiempo desde el �ltimo evento, lanzar� (desde Lua) un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/** Devuelve el tiempo (ms) global transcurrido */
		long getElapsedGlboalTime() const;

		/** Devuelve el tiempo (ms) de ronda transcurrido */
		long getElapsedRoundTime() const;

		/** Devuelve el tiempo (ms) de ronda restante */
		long getRemainingRoundTime() const;

		bool getPause() const
		{ return _pause; }

		void setPause(bool pause)
		{ _pause = pause; }

		/** Realiza las operaciones necesarias para comenzar la siguiente ronda */
		void startNextRound();

		/** 
		Altera el tiempo restante de la ronda actual 
		
		@param roundTimeChange Tiempo (ms) que se a�ade (>0) o quita (<0) al tiempo restante de ronda actual
		*/
		void changeCurrentRoundTime(int roundTimeChange);

	protected:
		CTimeManager();

		virtual ~CTimeManager();

		/**
		M�todo encargado de registrar en el contexto de Lua todas aquellas
		clases y funciones necesarias para el completo manejo de los eventos
		de juego desde Lua.
		*/
		void luaRegister();

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
		// TODO Leer desde configuraci�n externa
		/** Tiempo (ms) de ronda por defecto */
		const long int MAX_ROUND_TIME = 3 * 60 * 1000; // 3 minutos por ronda

		/**
		�nica instancia de la clase.
		*/
		static CTimeManager _instance;

		/** Tiempo (ms) de la ronda actual */
		long _maxRoundTime;

		/** Tiempo (ms) global transcurrido */
		long _globalTime;

		/** Tiempo (ms) de ronda transcurrido */
		long _roundTime;

		/** Flag a true si el tiempo est� pausado */
		bool _pause;

	};
}

#endif // TIME_MANAGER_H_