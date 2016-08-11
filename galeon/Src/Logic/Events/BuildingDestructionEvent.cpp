//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.cpp

Implementación de un evento de tipo INFO consistente en la destrucción de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#include "BuildingDestructionEvent.h"

#include "BaseSubsystems/ScriptManager.h"
#include "Logic/BuildingManager.h"

namespace Logic {

	void CBuildingDestructionEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CBuildingDestructionEvent, CEvent>("CBuildingDestructionEvent")
				.scope
				[
					luabind::def("addCBuildingDestructionEvent", &CBuildingDestructionEvent::addCBuildingDestructionEvent)
				]
			];
	}

	//--------------------------------------------------------

	std::string CBuildingDestructionEvent::getGUIImageName() const {
		return _image;
	}

	std::string CBuildingDestructionEvent::getGUITitle() const {
		return _title;
	}

	std::string CBuildingDestructionEvent::getGUIText() const {
		return _description;
	}

	std::string CBuildingDestructionEvent::getGUIResumeText() const {
		return "";
	}
	
	//--------------------------------------------------------

	void CBuildingDestructionEvent::execute()
	{
		if (_buildingType == BuildingType::Unassigned)
			Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();
		else
			Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding(_buildingType);

		// TODO ¿Notificar distinto dependiendo de buildingDestroyed?
	} // execute
	
} // namespace Logic
