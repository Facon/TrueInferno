//---------------------------------------------------------------------------
// GodBoostEvent.cpp
//---------------------------------------------------------------------------

/**
@file GodBoostEvent.cpp

Implementación de la clase CGodBoostEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "GodBoostEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/SoulManager.h"
#include "AI/Manager/AIManager.h"
#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CGodBoostEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CGodBoostEvent, CEvent>("CGodBoostEvent")
				.scope
				[
					luabind::def("addCGodBoostEvent", &CGodBoostEvent::addCGodBoostEvent)
				]
			];
	}

	//--------------------------------------------------------

	void CGodBoostEvent::execute()
	{
		_restore ? restore() : apply();

	} // execute

	//--------------------------------------------------------

	void CGodBoostEvent::apply()
	{
		// Alterar el ritmo de modificación del score del dios
		AI::CAIManager::getSingletonPtr()->getGod(_godName)->applyBoost(_factor);

		// Mostrar panel
		// @TODO Hacer esto bien...
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();

		uiManager->getEventUI()->setEventImage(_image);
		uiManager->getEventUI()->setEventTitle(_title);

		uiManager->getEventUI()->setEventText(_description);
		uiManager->getEventUI()->setEventTextResume("");
		uiManager->getEventUI()->setEventWindowVisible(true);

		// Crear el evento opuesto para restablecer los valores alterados
		CEventManager::getSingletonPtr()->addTimeEvent(
			new CGodBoostEvent(_time + _duration, _godName, 1.0 / _factor, true));

	} // apply

	//--------------------------------------------------------

	void CGodBoostEvent::restore()
	{
		// Restaurar el efecto del boost. El evento debería haberse construido con con el factor inverso
		AI::CAIManager::getSingletonPtr()->getGod(_godName)->removeBoost(_factor);

	} // restore
	
} // namespace Logic
