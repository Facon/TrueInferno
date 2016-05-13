//---------------------------------------------------------------------------
// RoundTimeChangeEvent.cpp
//---------------------------------------------------------------------------

/**
@file RoundTimeChangeEvent.cpp

Implementación de la clase CRoundTimeChangeEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "RoundTimeChangeEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/SoulManager.h"
#include "AI/Manager/AIManager.h"
#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CRoundTimeChangeEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CRoundTimeChangeEvent, CEvent>("CRoundTimeChangeEvent")
				.scope
				[
					luabind::def("addCRoundTimeChangeEvent", &CRoundTimeChangeEvent::addCRoundTimeChangeEvent)
				]
			];
	}

	//--------------------------------------------------------

	void CRoundTimeChangeEvent::execute()
	{
		CTimeManager::getSingletonPtr()->changeCurrentRoundTime(_roundTimeChange * 1000);

		// Mostrar panel
		// @TODO Hacer esto bien...
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();

		uiManager->getEventUI()->setEventImage(_image);
		uiManager->getEventUI()->setEventTitle(_title);

		uiManager->getEventUI()->setEventText(_description);
		uiManager->getEventUI()->setEventTextResume("");
		uiManager->getEventUI()->setEventWindowVisible(true);
	} // apply
	
} // namespace Logic
