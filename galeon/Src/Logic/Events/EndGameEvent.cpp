//---------------------------------------------------------------------------
// CEndGameEvent.cpp
//---------------------------------------------------------------------------

/**
@file CEndGameEvent.cpp

Implementación de un evento de tipo INFO consistente únicamente en mostrar
el panel de fin del juego (con victoria o derrota).

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#include "EndGameEvent.h"
#include "BaseSubsystems/ScriptManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CEndGameEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CEndGameEvent, CEvent>("CEndGameEvent")
				.def(luabind::constructor<bool>())
			];
	}

	//--------------------------------------------------------

	void CEndGameEvent::execute()
	{
		// @TODO Hacer esto bien...
		std::string eventImage, eventTitle, eventText, eventTextResume;

		switch (_victory)
		{
		case true:
			eventImage = "EventVictory";
			eventTitle = "VICTORY!";
			eventText = "YOU DID IT! You ARE the WINNER!";
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
