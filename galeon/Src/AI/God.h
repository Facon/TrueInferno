//---------------------------------------------------------------------------
// God.h
//---------------------------------------------------------------------------

/**
@file God.h

Contiene la declaración de la clase CGod.
Modela un dios, que son los enemigos del juego controlados por la IA.

@author Álvaro Valera
@date May, 2016
*/

#ifndef GOD_H_
#define GOD_H_

#include <string>

namespace AI {

	namespace GodMood {
		const int WORST_MOOD = -3;
		const int INITIAL_MOOD = 0;
		const int BEST_MOOD = 3;
	}

	class CGod {
	public:
		CGod(const std::string& name, bool isBoss);

		virtual ~CGod() {}

		/** Getter for name */
		const std::string getName() const { return _name; };

		/** Getter for score */
		const int getScore() const { return _score; };

		/** Getter for mood */
		const int getMood() const { return _mood; };

		/** Getter for isBoss */
		const bool isBoss() const { return _isBoss; };

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Si ha pasado determinado tiempo desde el último evento, lanzará (desde Lua) un evento aleatorio nuevo.

		@param msecs milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

	private:
		/** God's name */
		std::string _name;

		/** God's score */
		int _score;

		/** God's mood */
		int _mood;

		/** Flag set to true if this god is the boss */
		bool _isBoss;

		/** Target score for the round */
		int _targetScore;

		/** Tiempo(ms) mínimo entre actualizaciones de score */
		const int minTimeBetweenScoreUpdate = 3000;

		/** Tiempo(ms) máximo entre actualizaciones de score */
		const int maxTimeBetweenScoreUpdate = 5000;

		/** Tiempo(ms) que debe transcurrir hasta la próxima actualización de score */
		int _timeForNextScoreUpdate;

		/** Actualiza el tiempo hasta la siguiente actualización de score */
		void updateTimeForNextScoreUpdate();
	};
}

#endif GOD_H_