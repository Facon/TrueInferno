//---------------------------------------------------------------------------
// ObstacleGrowthEvent.cpp
//---------------------------------------------------------------------------

/**
@file ObstacleGrowthEvent.cpp

Implementación de la clase CObstacleGrowthEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "ObstacleGrowthEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/BuildingManager.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"
#include "Logic/Server.h"

namespace Logic {

	void CObstacleGrowthEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CObstacleGrowthEvent, CEvent>("CObstacleGrowthEvent")
				.scope
				[
					luabind::def("addCObstacleGrowthEvent", &CObstacleGrowthEvent::addCObstacleGrowthEvent)
				]
			];
	}

	//--------------------------------------------------------

	void CObstacleGrowthEvent::execute()
	{
		// Sacamos del TileManager la cantidad total de tiles
		int x, z;
		CTileManager::getSingletonPtr()->getDimensions(x, z);

		// Obtenemos el número de obstáculos deseado
		int numObstacles = _tileFactor * x * z;

		// Solicitamos al BuildingManager que haga brotar obstáculos
		CBuildingManager::getSingletonPtr()->growDestroyableObstaclesRandomly(CServer::getSingletonPtr()->getMap(), numObstacles);

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
