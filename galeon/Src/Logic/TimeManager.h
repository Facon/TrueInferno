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
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Si ha pasado determinado tiempo desde el último evento, lanzará (desde Lua) un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el último tick.
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

	protected:
		CTimeManager();

		virtual ~CTimeManager();

		/**
		Método encargado de registrar en el contexto de Lua todas aquellas
		clases y funciones necesarias para el completo manejo de los eventos
		de juego desde Lua.
		*/
		void luaRegister();

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
		static CTimeManager _instance;

		/** Tiempo (ms) inicial */
		static const long _maxRoundTime;

		/** Tiempo (ms) global transcurrido */
		long _globalTime;

		/** Tiempo (ms) de ronda transcurrido */
		long _roundTime;

		/** Flag a true si el tiempo está pausado */
		bool _pause;

	};
}

#endif // TIME_MANAGER_H_