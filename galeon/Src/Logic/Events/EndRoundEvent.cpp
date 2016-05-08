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
#include "GUI/EventUI.h"

namespace Logic {

	void CEndRoundEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CEndRoundEvent, CEvent>("CEndRoundEvent")
				.def(luabind::constructor<bool>())
			];
	}

	//--------------------------------------------------------

	void CEndRoundEvent::execute()
	{
		// @TODO Hacer esto bien...
		std::string eventImage, eventTitle, eventText, eventTextResume;

		switch (_victory)
		{
		case true:
			eventImage = "EventVictory";
			eventTitle = "You are in the NEXT ROUND!";
			eventText = "YES!! That’s how it’s done, kiddo. I KNEW you had potential. HAH!! Look at your rivals’ faces, you really pissed them off. This deserves some extra points. Here you go. Hades’ Favor increased! +66.666 Now on to the next round! Give’em Hell!";
			eventTextResume = "";
			break;

		case false:
			eventImage = "EventDefeat";
			eventTitle = "You have been ELIMINATED!";
			eventText = "Sad news: you failed. You’re done. Out. FIRED. And I thought you had potential... I hate being wrong. I really do. Now get the Hell out of my Hell before I throw you in a furnace.";
			eventTextResume = "";
			break;
		}

		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getEventUI()->setEventImage(eventImage);
		uiManager->getEventUI()->setEventTitle(eventTitle);
		uiManager->getEventUI()->setEventText(eventText);
		uiManager->getEventUI()->setEventTextResume(eventTextResume);
		uiManager->getEventUI()->setEventWindowVisible(true);

	} // execute
	
} // namespace Logic
