//---------------------------------------------------------------------------
// Player.h
//---------------------------------------------------------------------------

/**
@file Player.h

Contiene la declaración de la clase CPlayer.
Esta clase representa lógicamente al jugador en el juego.

@author Álvaro Valera
@date May, 2016
*/

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

#include "Logic\HFManager.h"
#include "RankedCharacter.h"

namespace AI {

	class CPlayer : public IRankedCharacter {
	public:
		CPlayer() : _name("You") {}

		/** Getter for name */
		const std::string& getName() const {
			return _name;
		}

		/** Getter for score */
		virtual const int getScore() const {
			// Reads score directly from the specific Manager
			return Logic::HFManager::getSingletonPtr()->getHadesFavor();
		}

	private:
		/** Nombre del jugador */
		std::string _name;
	};
}

#endif PLAYER_H_