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

#include "God.h"
#include "Logic\TimeManager.h"
#include <cstdlib>

namespace AI {
	CGod::CGod(const std::string& name, bool isBoss) : _name(name), _score(0), _mood(GodMood::INITIAL_MOOD), _isBoss(isBoss) {
		// TODO TEST
		_targetScore = 500;

		// Inicializamos el tiempo para la siguiente actualización de score
		updateTimeForNextScoreUpdate();
	}

	void CGod::updateTimeForNextScoreUpdate(){
		_timeForNextScoreUpdate = std::rand() % (maxTimeBetweenScoreUpdate - minTimeBetweenScoreUpdate + 1) + minTimeBetweenScoreUpdate;
	}

	void CGod::tick(unsigned int msecs) {
		// Si no es el jefe y toca actualizar score
		if((_isBoss==false) && ((_timeForNextScoreUpdate -= msecs) < 0)){
			// TODO Añadimos cierto valor aleatorio a la puntuación actual como si el dios hubiera hecho algo. Coger trazas para simular humano!
			//_score += 0;

			// Calculamos la velocidad necesaria para llegar a la puntuación objetivo en el tiempo de ronda restante
			long roundRemainingTime = Logic::TimeManager::getSingletonPtr()->getRemainingTime();
			float scoreSpeed = (_targetScore - _score) / (float)roundRemainingTime;

			// Calculamos nuevo tiempo ara la siguiente actualización de score
			updateTimeForNextScoreUpdate();

			// Avanzamos la puntuación según el tiempo que transcurrirá
			_score += scoreSpeed * _timeForNextScoreUpdate;
		}

	} // tick
}