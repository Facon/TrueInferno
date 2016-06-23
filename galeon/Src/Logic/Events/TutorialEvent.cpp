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

namespace Logic {

	void CTutorialEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CTutorialEvent, CEvent>("CTutorialEvent")
				.scope
				[
					luabind::def("addCTutorialEvent", &CTutorialEvent::addCTutorialEvent)
				]
			];
	}

	//--------------------------------------------------------

	std::string CTutorialEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CTutorialEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CTutorialEvent::getGUIText() const {
		return _eventText;
	}

	std::string CTutorialEvent::getGUIResumeText() const {
		return _eventTextResume;
	}
	
	//--------------------------------------------------------

	void CTutorialEvent::execute()
	{
	} // execute
	
	void CTutorialEvent::initGUIConstants(){
		switch (_tutorialPanelIndex)
		{
		case 1:
			_eventImage = "EventTutorial1";
			_eventTitle = "Welcome to Hell";
			_eventText = "Well, well… Welcome to Hell! The Pandemonium is about to start, so... What, you still don’t know how this works? Oh boy… I love new guys. So clueless, so innocent… Alright gang, explain the basics for our newcomer. Be kind for a minute. I know it hurts, but you’ll live. Good luck now, kiddo. Remember, I’ll be watching you.";
			_eventTextResume = "Reach 10000 points, if you can...";
			break;

		case 2:
			_eventImage = "EventTutorial2";
			_eventTitle = "Building facilities";
			_eventText = "Oh, great… The rookie. I really don’t get why we must put up with these losers. Look at this one… Hey you, where do you think you are? Heaven? Come on, move! Start building something!";
			_eventTextResume = "Press on a building’s button to select it, then click anywhere on the terrain to build it there.";
			break;

		case 3:
			_eventImage = "EventTutorial3";
			_eventTitle = "Building soulpaths";
			_eventText = "Please do not mind Lucy’s manners. She forgets she was also a rookie once, just as everyone here. Except me, of course. My pleasure, I’m Osiris. You will learn a lot from me if you pay attention. Here, you should connect your facilities with Soulpaths so your personnel can move around.";
			_eventTextResume = "Press the Soulpath button and draw a line on the terrain.";
			break;

		case 4:
			_eventImage = "EventTutorial4";
			_eventTitle = "Putting souls to work";
			_eventText = "Naphlegeth grah'n y-ehye hafh'drnoth gotha chtenff fm'latgh nog, ilyaa bug llll nan'gha y - vulgtlagln fm'latghog nnnlw'nafh ya nilgh'ri vulgtm nw nnnsgn'wahl chtenff ph'wgah'n.";
			_eventTextResume = "Er… Just do what he said, nothing to add.";
			break;

		case 5:
			_eventImage = "EventTutorial5";
			_eventTitle = "Putting souls to work";
			_eventText = "Don’t make faces. The Old One is easy to understand… if you listen with an open mind. He just said you’ve laid a path for your souls, and now you must point them to their eternal destiny. Trust Hel, I knows people.";
			_eventTextResume = "Press the button with the worker icon to send your souls to work.";
			break;

		case 9:
			_eventImage = "EventVictory";
			_eventTitle = "You are in the NEXT ROUND!";
			//_eventText = "YES!! That’s how it’s done, kiddo. I KNEW you had potential. HAH!! Look at your rivals’ faces, you really pissed them off. This deserves some extra points. Here you go. Hades’ Favor increased! +66.666 Now on to the next round! Give’em Hell!";
			_eventText = "I KNEW you had potential.HAH!!Look at your rivals’ faces. Hades’ Favor increased + 66.666! Now into the next round, Give’em Hell!";
			_eventTextResume = "";
			break;

		default:
			assert(false && "No values defined for GUI");
			_eventImage = "";
			_eventTitle = "";
			_eventText = "";
			_eventTextResume = "";
		}
	}

} // namespace Logic
