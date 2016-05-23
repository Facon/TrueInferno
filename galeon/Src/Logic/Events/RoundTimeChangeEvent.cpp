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

	std::string CRoundTimeChangeEvent::getGUIImageName() const {
		return _image;
	}

	std::string CRoundTimeChangeEvent::getGUITitle() const {
		return _title;
	}

	std::string CRoundTimeChangeEvent::getGUIText() const {
		return _description;
	}

	std::string CRoundTimeChangeEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CRoundTimeChangeEvent::execute()
	{
		CTimeManager::getSingletonPtr()->changeCurrentRoundTime(_roundTimeChange * 1000);
	} // execute
	
} // namespace Logic
