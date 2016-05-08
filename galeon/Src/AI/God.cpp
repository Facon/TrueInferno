//---------------------------------------------------------------------------
// God.cpp
//---------------------------------------------------------------------------

/**
@file God.cpp

Contiene la implementación de la clase CGod. 
Modela un dios, que son los enemigos del juego controlados por la IA.

@see Logic::CScriptManager

@author Álvaro Valera
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

		// Inicializamos el tiempo para la siguiente actualización de score
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
		// Si es el jefe, está eliminado o ha alcanzado la puntuación objetivo, no hacemos nada
		if (_isBoss || _isEliminated || (_score >= _targetScore))
			return;
		
		// Reducimos el tiempo restante la siguiente actualización de score
		_remainingTimeForNextScoreUpdate -= msecs;

		// Si toca actualizar ya
		if (_remainingTimeForNextScoreUpdate <= 0){
			// TODO Añadimos cierto valor aleatorio a la puntuación actual como si el dios hubiera hecho algo. Coger trazas para simular humano!
			//_score += 0;

			// Obtenemos el tiempo restante de ronda
			long roundRemainingTime = Logic::CTimeManager::getSingletonPtr()->getRemainingRoundTime();

			// Si no queda tiempo en la ronda no se hace nada
			if (roundRemainingTime == 0)
				return;

			// Calculamos la velocidad necesaria (puntos/s) para llegar a la puntuación objetivo en el tiempo de ronda restante
			float scoreSpeed = (_targetScore - _score) / (float)roundRemainingTime;
	
			// Avanzamos la puntuación a la velocidad calculada durante un periodo de aleatorio o todo lo posible
			_score += (int) (scoreSpeed * std::min(roundRemainingTime, (long)getRandomTimeForNextScoreUpdate()));

			// Obtenemos nuevo tiempo la siguiente actualización de score
			_remainingTimeForNextScoreUpdate = getRandomTimeForNextScoreUpdate();
		}

	} // tick
}