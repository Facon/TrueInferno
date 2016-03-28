//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.cpp

Implementaci�n de un evento de tipo INFO consistente en la destrucci�n de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#include "BuildingDestructionEvent.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

#include "Logic/BuildingManager.h"

namespace Logic {

	void CBuildingDestructionEvent::execute()
	{
		bool buildingDestroyed = Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();

		// Mostrar panel de destrucci�n de edificio
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
