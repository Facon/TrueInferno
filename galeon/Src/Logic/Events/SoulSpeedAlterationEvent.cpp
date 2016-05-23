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
#include "AI/God.h"

namespace Logic {

	void CSoulSpeedAlterationEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CSoulSpeedAlterationEvent, CEvent>("CSoulSpeedAlterationEvent")
				.scope
				[
					luabind::def("addCSoulSpeedAlterationEvent", &CSoulSpeedAlterationEvent::addCSoulSpeedAlterationEvent)
				]
			];
	}

	//--------------------------------------------------------

	std::string CSoulSpeedAlterationEvent::getGUIImageName() const {
		return _image;
	}

	std::string CSoulSpeedAlterationEvent::getGUITitle() const {
		return _title;
	}

	std::string CSoulSpeedAlterationEvent::getGUIText() const {
		return _description;
	}

	std::string CSoulSpeedAlterationEvent::getGUIResumeText() const {
		return "";
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

		// Creamos el evento opuesto para restablecer los valores alterados
		CEventManager::getSingletonPtr()->addTimeEvent(
			new CSoulSpeedAlterationEvent(_time + _duration, _god->getName(), "", "", "",  1.0 / _factor, 0, true, true));

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
