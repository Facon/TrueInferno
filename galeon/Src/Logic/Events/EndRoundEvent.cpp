//---------------------------------------------------------------------------
// CEndRoundEvent.cpp
//---------------------------------------------------------------------------

/**
@file CEndRoundEvent.cpp

Implementación de un evento de tipo INFO que muestra al jugador
el desenlace de la ronda (victoria o derrota).

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "EndRoundEvent.h"

#include "BaseSubsystems/ScriptManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/RankUI.h"

namespace Logic {
	std::string CEndRoundEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CEndRoundEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CEndRoundEvent::getGUIText() const {
		return _eventText;
	}

	std::string CEndRoundEvent::getGUIResumeText() const {
		return "";
	}

	//--------------------------------------------------------

	void CEndRoundEvent::execute()
	{
	} // execute
	
	void CEndRoundEvent::initGUIConstants(){
		if(_victory) {
			_eventImage = "EventVictory";
			//_eventTitle = "You are in the NEXT ROUND!";
			//_eventText = "YES!! That’s how it’s done, kiddo. I KNEW you had potential. HAH!! Look at your rivals’ faces, you really pissed them off. This deserves some extra points. Here you go. Hades’ Favor increased! +66.666 Now on to the next round! Give’em Hell!";
			_eventTitle = "NEXT ROUND!";
			_eventText = "YES!! I KNEW you had potential.HAH!!Look at your rivals’ faces. Hades’ Favor increased + 66.666! Now into the next round, Give em Hell!";
			GUI::CServer::getSingletonPtr()->getUIManager()->getRankUI()->round++;
			GUI::CServer::getSingletonPtr()->getUIManager()->getRankUI()->start = true;
		}

		else {
			_eventImage = "EventDefeat";
			_eventTitle = "You have been ELIMINATED!";
			_eventText = "Sad news: you failed. You’re done. Out. FIRED. And I thought you had potential... I hate being wrong. I really do. Now get the Hell out of my Hell before I throw you in a furnace.";
			_eventTextResume = "";
		}
	}

} // namespace Logic
