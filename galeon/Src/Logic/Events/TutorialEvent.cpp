//---------------------------------------------------------------------------
// TutorialEvent.cpp
//---------------------------------------------------------------------------

/**
@file TutorialEvent.cpp

Implementación de un evento de tipo INFO consistente únicamente en mostrar
uno de los paneles del tutorial.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#include "TutorialEvent.h"
#include "BaseSubsystems/ScriptManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CTutorialEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CTutorialEvent, CEvent>("CTutorialEvent")
				.def(luabind::constructor<int>())
			];
	}

	//--------------------------------------------------------

	void CTutorialEvent::execute()
	{
		// @TODO Hacer esto bien...
		std::string eventImage, eventTitle, eventText, eventTextResume;

		switch (_tutorialPanelIndex)
		{
		case 1:
			eventImage = "EventTutorial1";
			eventTitle = "Welcome to Hell";
			eventText = "Well, well… Welcome to Hell! The Pandemonium is about to start, so... What, you still don’t know how this works? Oh boy… I love new guys. So clueless, so innocent… Alright gang, explain the basics for our newcomer. Be kind for a minute. I know it hurts, but you’ll live. Good luck now, kiddo. Remember, I’ll be watching you.";
			eventTextResume = "Reach 10000 points, if you can...";
			break;
		case 2:
			eventImage = "EventTutorial2";
			eventTitle = "Building facilities";
			eventText = "Oh, great… The rookie. I really don’t get why we must put up with these sissies. Look at this one… Hey you, where do you think you are? Heaven? Come on, move! Start building something!";
			eventTextResume = "Press on a building’s button to select it, then click anywhere on the terrain to build it there.";
			break;
		case 3:
			eventImage = "EventTutorial3";
			eventTitle = "Building soulpaths";
			eventText = "Please do not mind Lucy’s manners. She forgets she was also a rookie once, just as everyone here. Except me, of course. My pleasure, I’m Osiris. You will learn a lot from me if you pay attention. Here, you should connect your facilities with Soulpaths so your personnel can move around.";
			eventTextResume = "Press the Soulpath button and draw a line on the terrain.";
			break;
		case 4:
			eventImage = "EventTutorial4";
			eventTitle = "Putting souls to work";
			eventText = "Naphlegeth grah'n y-ehye hafh'drnoth gotha chtenff fm'latgh nog, ilyaa bug llll nan'gha y - vulgtlagln fm'latghog nnnlw'nafh ya nilgh'ri vulgtm nw nnnsgn'wahl chtenff ph'wgah'n.";
			eventTextResume = "Er… Just do what he said, nothing to add.";
			break;
		case 5:
			eventImage = "EventTutorial5";
			eventTitle = "Putting souls to work";
			eventText = "Don’t make faces. The Old One is easy to understand… if you listen with an open mind. He just said you’ve laid a path for your souls, and now you must point them to their eternal destiny. Trust Hel, I knows people.";
			eventTextResume = "Press the button with the worker icon to send your souls to work.";
			break;
		case 9:
			eventImage = "EventVictory";
			eventTitle = "You are in the NEXT ROUND!";
			eventText = "YES!! That’s how it’s done, kiddo. I KNEW you had potential. HAH!! Look at your rivals’ faces, you really pissed them off. This deserves some extra points. Here you go. Hades’ Favor increased! +66.666 Now on to the next round! Give’em Hell!";
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
