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
#include <Audio/Server.h>

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

	std::string CEndGameEvent::getGUIImageName() const {
		return _eventImage;
	}

	std::string CEndGameEvent::getGUITitle() const {
		return _eventTitle;
	}

	std::string CEndGameEvent::getGUIText() const {
		return _eventText;
	}

	std::string CEndGameEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CEndGameEvent::execute()
	{
		if (_victory)
			Audio::CServer::getSingletonPtr()->playSound("victory", 0.4f);
		else
			Audio::CServer::getSingletonPtr()->playSound("error", 0.4f);
	} // execute

	void CEndGameEvent::initGUIConstants(){
		if (_victory) {
			_eventImage = "EventVictory";
			_eventTitle = "VICTORY!";
			_eventText = "YOU DID IT! You ARE the WINNER!";
			_eventTextResume = "";
		}

		else{
			_eventImage = "EventDefeat";
			_eventTitle = "You have been ELIMINATED!";
			_eventText = "Sad news: you failed. You’re done. Out. FIRED. And I thought you had potential... I hate being wrong. I really do. Now get the Hell out of my Hell before I throw you in a furnace.";
			_eventTextResume = "";
		}
	}
	
} // namespace Logic
