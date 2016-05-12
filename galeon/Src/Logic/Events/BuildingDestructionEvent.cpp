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

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

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

	void CBuildingDestructionEvent::execute()
	{
		bool buildingDestroyed = Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();

		// Mostrar panel de destrucción de edificio
		// @TODO Hacer esto bien...
		if (buildingDestroyed) {
			GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
			uiManager->getEventUI()->setEventImage("EventBuildingDestruction");
			uiManager->getEventUI()->setEventTitle("Building Destruction");
			uiManager->getEventUI()->setEventText("A building has been destroyed!");
			uiManager->getEventUI()->setEventTextResume("");
			uiManager->getEventUI()->setEventWindowVisible(true);
		}

	} // execute
	
} // namespace Logic
