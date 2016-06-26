#ifndef GAME_RUNTIME_CONTEXT_H_
#define GAME_RUNTIME_CONTEXT_H_

namespace Logic 
{
		/** Contexto de ejecuci�n del juego */
	enum GameRuntimeContext
	{
		// Juego real (contexto por defecto)
		GAME = 0,
	
		// Demo scriptada y controlada (e.g. eventos programados, dificultad imposible para la 2� ronda, etc.)
		SCRIPTED_DEMO,

		// Desarrollo (e.g. mayor cantidad de recursos iniciales, tiempo m�ximo por ronda, etc.)
		DEV
	};
}

#endif