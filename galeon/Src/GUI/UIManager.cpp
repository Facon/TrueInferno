#include "UIManager.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>


#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"

namespace GUI
{
	UIManager::UIManager()
	{
	}


	UIManager::~UIManager()
	{
	}


	void UIManager::init()
	{
		// Cargamos la ventana que muestra la barra superior
		


		_uiWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UI.layout");

		_resourcesUI.init();

		_sidebarUI.init();

		_uiWindow->addChild(_resourcesUI.getResourceWindow());
		_uiWindow->addChild(_sidebarUI.getSideBarWindow());

		
	}

	void UIManager::activate()
	{
		// Activamos la interfaz de usuario
		
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uiWindow);


		_resourcesUI.activate();
		_sidebarUI.activate();
		_uiWindow->setVisible(true);
		_uiWindow->activate();

	}

	void UIManager::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		
		_uiWindow->deactivate();
		_uiWindow->setVisible(false);
		_resourcesUI.deactivate();
		_sidebarUI.deactivate();

	}

	void UIManager::tick(unsigned int msecs)
	{
		_resourcesUI.tick(msecs);
		_sidebarUI.tick(msecs);
	}



}