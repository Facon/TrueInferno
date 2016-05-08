//---------------------------------------------------------------------------
// God.cpp
//---------------------------------------------------------------------------

/**
@file God.cpp

Contiene la implementaci�n de la clase CGod. 
Modela un dios, que son los enemigos del juego controlados por la IA.

@see Logic::CScriptManager

@author �lvaro Valera
@date Abril, 2016
*/

#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "God.h"

#include "Logic\TimeManager.h"

namespace AI {
	CGod::CGod(const std::string& name, bool isBoss) : 
		_name(name), _score(0), _targetScore(0), _mood(GodMood::INITIAL_MOOD), _isBoss(isBoss), _isEliminated(false) {

		// Inicializamos el tiempo para la siguiente actualizaci�n de score
		_remainingTimeForNextScoreUpdate = getRandomTimeForNextScoreUpdate();
	}

	int CGod::getRandomTimeForNextScoreUpdate(){
		return std::rand() % (maxTimeBetweenScoreUpdate - minTimeBetweenScoreUpdate + 1) + minTimeBetweenScoreUpdate;
	}

	void CGod::eliminate(){
		assert(!_isEliminated && "Can't eliminate an already eliminated god");

		_isEliminated = true;
	}

	void CGod::tick(unsigned int msecs) {
		// Si es el jefe, est� eliminado o ha alcanzado la puntuaci�n objetivo, no hacemos nada
		if (_isBoss || _isEliminated || (_score >= _targetScore))
			return;
		
		// Reducimos el tiempo restante la siguiente actualizaci�n de score
		_remainingTimeForNextScoreUpdate -= msecs;

		// Si toca actualizar ya
		if (_remainingTimeForNextScoreUpdate <= 0){
			// TODO A�adimos cierto valor aleatorio a la puntuaci�n actual como si el dios hubiera hecho algo. Coger trazas para simular humano!
			//_score += 0;

			// Obtenemos el tiempo restante de ronda
			long roundRemainingTime = Logic::CTimeManager::getSingletonPtr()->getRemainingRoundTime();

			// Si no queda tiempo en la ronda no se hace nada
			if (roundRemainingTime == 0)
				return;

			// Calculamos la velocidad necesaria (puntos/s) para llegar a la puntuaci�n objetivo en el tiempo de ronda restante
			float scoreSpeed = (_targetScore - _score) / (float)roundRemainingTime;
	
			// Avanzamos la puntuaci�n a la velocidad calculada durante un periodo de aleatorio o todo lo posible
			_score += (int) (scoreSpeed * std::min(roundRemainingTime, (long)getRandomTimeForNextScoreUpdate()));

			// Obtenemos nuevo tiempo la siguiente actualizaci�n de score
			_remainingTimeForNextScoreUpdate = getRandomTimeForNextScoreUpdate();
		}

	} // tick
}