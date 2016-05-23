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
#include "AI/Manager/AIManager.h"

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

	std::string CGodBoostEvent::getGUIImageName() const {
		return _image;
	}

	std::string CGodBoostEvent::getGUITitle() const {
		return _title;
	}

	std::string CGodBoostEvent::getGUIText() const {
		return _description;
	}

	std::string CGodBoostEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CGodBoostEvent::execute()
	{
		_restore ? restore() : apply();

	} // execute

	//--------------------------------------------------------

	void CGodBoostEvent::apply()
	{
		if (_god == nullptr){
			assert(false && "There must be a related god");
			return;
		}

		// Alterar el ritmo de modificación del score del dios
		_god->applyBoost(_factor);

		// Crear el evento opuesto para restablecer los valores alterados
		CEventManager::getSingletonPtr()->addTimeEvent(
			new CGodBoostEvent(_time + _duration, _god->getName(), 1.0 / _factor, true));

	} // apply

	//--------------------------------------------------------

	void CGodBoostEvent::restore()
	{
		if (_god == nullptr){
			assert(false && "There must be a related god");
			return;
		}

		// Restaurar el efecto del boost. El evento debería haberse construido con con el factor inverso
		_god->removeBoost(_factor);

	} // restore
	
} // namespace Logic
