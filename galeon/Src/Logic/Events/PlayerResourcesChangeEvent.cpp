//---------------------------------------------------------------------------
// PlayerResourcesChangeEvent.cpp
//---------------------------------------------------------------------------

/**
@file PlayerResourcesChangeEvent.cpp

Implementación de la clase CPlayerResourcesChangeEvent.

@see Logic::CEvent

@author Álvaro Valera
@date Mayo, 2016
*/

#include "PlayerResourcesChangeEvent.h"

#include "EventManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/SoulManager.h"
#include "AI/Manager/AIManager.h"
#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"

namespace Logic {

	void CPlayerResourcesChangeEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CPlayerResourcesChangeEvent, CEvent>("CPlayerResourcesChangeEvent")
				.scope
				[
					luabind::def("addCPlayerResourcesChangeEvent", &CPlayerResourcesChangeEvent::addCPlayerResourcesChangeEvent)
				]
			];
	}

	//--------------------------------------------------------

	void CPlayerResourcesChangeEvent::execute()
	{
		// Preparamos la variable que contendrá el resultado del incremento/decremento
		int result = 0;

		ResourcesManager *resourcesManager = ResourcesManager::getSingletonPtr();

		// Damos o quitamos recursos sobre la cantidad almacenada
		if (std::abs(_resourceStoredFactor) > ZERO_FACTOR) {
			// Obtenemos la cantidad total de recursos almacenados
			int storedResources = resourcesManager->getDisplayedResources(_resourceType);

			// Calculamos la cantidad a añadir/quitar aplicando el factor
			int quantity = std::abs((int)(_resourceStoredFactor * storedResources));

			if (_resourceStoredFactor > 0)
				resourcesManager->incrementResources(_resourceType, quantity, false, true, result);

			else // _resourceStoredFactor < 0
				resourcesManager->decrementResources(_resourceType, quantity, false, true, result);
		}

		// Damos o quitamos recursos sobre la capacidad de almacenamiento
		if (std::abs(_resourceStorageFactor) > ZERO_FACTOR) {
			// Obtenemos capacidad de almacenamiento total
			int resourceStorage = 
				resourcesManager->getDisplayedMaxResources(_resourceType) - resourcesManager->getDisplayedResources(_resourceType);

			// Calculamos la cantidad a añadir/quitar aplicando el factor
			int quantity = std::abs((int)(_resourceStorageFactor * resourceStorage));

			if (_resourceStorageFactor > 0)
				resourcesManager->incrementResources(_resourceType, quantity, false, true, result);

			else // _resourceStorageFactor < 0
				resourcesManager->decrementResources(_resourceType, quantity, false, true, result);
		}

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
