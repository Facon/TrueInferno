//---------------------------------------------------------------------------
// RankedCharacter.h
//---------------------------------------------------------------------------

/**
@file RankedCharacter.h

Contiene la declaración de la interfaz IRankedCharacter.
Esta interfaz define los métodos necesarios para que "algo" (e.g. un dios, el jugador)
se pueda mostrar en un ranking.

@author Álvaro Valera
@date May, 2016
*/

#ifndef RANKED_CHARACTER_H_
#define RANKED_CHARACTER_H_

#include <string>

namespace AI {

	class IRankedCharacter {
	public:
		/** Getter for name */
		virtual const std::string& getName() const = 0;

		/** Getter for score */
		virtual const int getScore() const = 0;
	};
}

#endif RANKED_CHARACTER_H_