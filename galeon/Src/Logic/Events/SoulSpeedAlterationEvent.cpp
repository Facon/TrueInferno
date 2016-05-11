//---------------------------------------------------------------------------
// SoulSpeedAlterationEvent.cpp
//---------------------------------------------------------------------------

/**
@file SoulSpeedAlterationEvent.cpp

Implementación de un evento de tipo INFO consistente en alterar la
velocidad de movimiento y de recolección de recursos de las almas
durante un cierto tiempo.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "SoulSpeedAlterationEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/SoulManager.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CSoulSpeedAlterationEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CSoulSpeedAlterationEvent, CEvent>("CSoulSpeedAlterationEvent")
				.def(luabind::constructor<long, float, int, bool>())
			];
	}

	//--------------------------------------------------------

	void CSoulSpeedAlterationEvent::execute()
	{
		_restore ? restore() : apply();

	} // execute

	//--------------------------------------------------------

	void CSoulSpeedAlterationEvent::apply()
	{
		// Alterar la velocidad de movimiento de las almas
		Logic::CSoulManager::getSingletonPtr()->modifyMovementSpeed(_factor);

		// Alterar la velocidad de recolección de recursos de las almas
		// @TODO

		// Mostrar panel dependiendo de si se aumenta o reduce la velocidad
		std::string eventText = "";

		// @TODO Hacer esto bien...
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		if (_factor < 1.0) {
			eventText += "The movement and gathering speed of your souls has been decreased, but ";
			eventText += "don't worry, they will be fine again soon...hopefully...";

			uiManager->getEventUI()->setEventImage("EventSoulsSpeedReduction");
			uiManager->getEventUI()->setEventTitle("Useless souls");
		}

		else{
			eventText += "The movement and gathering speed of your souls has been temporally increased! ";
			eventText += "Go, souls, go!";

			// TODO Reemplazar por imagen correcta
			uiManager->getEventUI()->setEventImage("EventSoulsSpeedReduction");
			uiManager->getEventUI()->setEventTitle("Useful souls");
		}

		uiManager->getEventUI()->setEventText(eventText);
		uiManager->getEventUI()->setEventTextResume("");
		uiManager->getEventUI()->setEventWindowVisible(true);

		// Crear el evento opuesto para restablecer los valores alterados
		CEventManager::getSingletonPtr()->addTimeEvent(
			new CSoulSpeedAlterationEvent(_time + _duration, 1.0 / _factor, 0, true, true));

	} // apply

	//--------------------------------------------------------

	void CSoulSpeedAlterationEvent::restore()
	{
		// Restaurar la velocidad de movimiento de las almas. El evento debería haberse construido con con el factor inverso
		Logic::CSoulManager::getSingletonPtr()->modifyMovementSpeed(_factor);

		// Restaurar la velocidad de recolección de recursos de las almas
		// @TODO

	} // restore
	
} // namespace Logic
