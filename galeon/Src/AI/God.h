//---------------------------------------------------------------------------
// God.h
//---------------------------------------------------------------------------

/**
@file God.h

Contiene la declaraci�n de la clase CGod.
Modela un dios, que son los enemigos del juego controlados por la IA.

@author �lvaro Valera
@date May, 2016
*/

#ifndef GOD_H_
#define GOD_H_

#include <string>
#include <iostream>

#include "RankedCharacter.h"

namespace AI {

	namespace GodMood {
		const int WORST_MOOD = -3;
		const int INITIAL_MOOD = 0;
		const int BEST_MOOD = 3;
	}

	class CGod : public IRankedCharacter {
	public:
		CGod(const std::string& name, bool isBoss);

		virtual ~CGod() {}

		/** Getter for name */
		const std::string& getName() const { return _name; };

		/** Getter for score */
		const int getScore() const { return _score; };

		/** Getter for mood */
		const int getMood() const { return _mood; };

		/** Getter for isBoss */
		const bool isBoss() const { return _isBoss; };

		/** Getter for isEliminated */
		const bool isEliminated() const { return _isEliminated; };

		/** Getter for targetScore */
		const int getTargetScore() const { return _targetScore; };

		/** Setter for targetScore */
		void setTargetScore(int targetScore) {
			_targetScore = targetScore;

			/*if (!_isBoss && !_isEliminated)
				std::cout << _name << ": " << _score << " -> " << _targetScore << std::endl;*/
		};

		/** Eliminates this god */
		void eliminate();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Si ha pasado determinado tiempo desde el �ltimo evento, lanzar� (desde Lua) un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/** Cambia la puntuaci�n del dios seg�n la cantidad (positiva o negativa) indicada */
		void changeScore(int change);

		/**
		Aplica un factor aditivo a la velcoidad base de modificaci�n del score del dios:
		velocidad = (1 + boostFactor) * velocidadBase

		e.g. Si boostFactor = 1 => velocidad = (1 + 1) * velocidadBase = 2 * velocidadBase = 200% velocidadBase (velocidad duplicada)
		e.g. Si boostFactor = 2 => velocidad = (1 + 2) * velocidadBase = 3 * velocidadBase = 300% velocidadBase (velocidad triplicada)
		e.g. Si boostFactor = -1 => velocidad = (1 - 1) * velocidadBase = 0 * velocidadBase = 0% velocidadBase (velocidad anulada)
		e.g. Si boostFactor = -2 => velocidad = (1 - 2) * velocidadBase = -1 * velocidadBase = -100% velocidadBase (velocidad negativa!)
		e.g. Si boostFactor = -0.5 => velocidad = (1 - 0.5) * velocidadBase = 0.5 * velocidadBase = 50% velocidadBase (velocidad a la mitad)

		@param factor Factor por el cual se modifica el ritmo de crecimiento del score
		*/
		void applyBoost(float boostFactor);

		/**
		Deshace la operaci�n realizada en applyBoost

		@param factor Factor a deshacer
		*/
		void removeBoost(float boostFactor);

	private:
		/** Nombre del dios */
		std::string _name;

		/** Score del dios. Para modificarlo usar changeScore */
		int _score;

		/** Rollito del dios */
		int _mood;

		/** Flag set to true if this god is the boss */
		bool _isBoss;

		/** Target score for the round */
		int _targetScore;

		/** Tiempo(ms) m�nimo hasta la primera actualizaci�n de score */
		const int minTimeForFirstScoreUpdate = 10000;
		// TODO TEST
		//const int minTimeForFirstScoreUpdate = 30000;
		
		/** Tiempo(ms) m�nimo hasta la primera actualizaci�n de score */
		const int maxTimeForFirstScoreUpdate = 10000;
		// TODO TEST
		//const int maxTimeForFirstScoreUpdate = 40000;

		/** Tiempo(ms) m�nimo entre actualizaciones de score */
		const int minTimeBetweenScoreUpdate = 3000;

		/** Tiempo(ms) m�ximo entre actualizaciones de score */
		const int maxTimeBetweenScoreUpdate = 6000;

		/** Tiempo(ms) restante para la pr�xima actualizaci�n de score */
		int _remainingTimeForNextScoreUpdate;

		/**
		Obtiene un tiempo aleatorio la siguiente actualizaci�n de score 
		
		@param minTime rango inferior m�mimo para el tiempo aleatorio
		@param maxTime rango inferior m�ximo para el tiempo aleatorio
		*/
		int getRandomTimeForNextScoreUpdate(int minTime, int maxTime);

		/** Flag a true cuando el dios est� eliminado */
		bool _isEliminated;

		/** Factor que altera el ritmo de modificaci�n del score del dios */
		float _boostFactor;
	};
}

#endif GOD_H_