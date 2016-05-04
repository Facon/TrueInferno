//---------------------------------------------------------------------------
// SoulsSpeedReductionEvent.cpp
//---------------------------------------------------------------------------

/**
@file SoulsSpeedReductionEvent.cpp

Implementación de un evento de tipo INFO consistente en ralentizar la
velocidad de movimiento y de recolección de recursos de las almas
durante un cierto tiempo.

@see Logic::CEvent

@author Raúl Segura
@date Abril, 2016
*/

#include "SoulsSpeedReductionEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/SoulManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CSoulsSpeedReductionEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CSoulsSpeedReductionEvent, CEvent>("CSoulsSpeedReductionEvent")
				.def(luabind::constructor<long, bool>())
				.def(luabind::constructor<ConditionTriggerType, bool, bool>())
			];
	}

	//--------------------------------------------------------

	void CSoulsSpeedReductionEvent::execute()
	{
		_restore ? restore() : apply();

	} // execute

	//--------------------------------------------------------

	void CSoulsSpeedReductionEvent::apply()
	{
		// Reducir la velocidad de movimiento de las almas
		Logic::CSoulManager::getSingletonPtr()->modifyMovementSpeed(0.25f);

		// Reducir la velocidad de recolección de recursos de las almas
		// @TODO

		// Mostrar panel de ralentización de almas
		std::string eventText("The movement and gathering speed of your souls has been decreased, but ");
		eventText += "don't worry, they will be fine again soon...hopefully...";

		// @TODO Hacer esto bien...
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getEventUI()->setEventImage("EventSoulsSpeedReduction");
		uiManager->getEventUI()->setEventTitle("Useless souls");
		uiManager->getEventUI()->setEventText(eventText);
		uiManager->getEventUI()->setEventTextResume("");
		uiManager->getEventUI()->setEventWindowVisible(true);

		// Crear el evento opuesto para restablecer los valores alterados
		CEventManager::getSingletonPtr()->addTimeEvent(
			new CSoulsSpeedReductionEvent(_time + 10 * 1000, true, true));

	} // apply

	//--------------------------------------------------------

	void CSoulsSpeedReductionEvent::restore()
	{
		// Restaurar la velocidad de movimiento de las almas
		Logic::CSoulManager::getSingletonPtr()->restoreMovementSpeed();

		// Restaurar la velocidad de recolección de recursos de las almas
		// @TODO

	} // restore
	
} // namespace Logic
