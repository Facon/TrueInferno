#include "UIManager.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

namespace GUI
{
	UIManager::UIManager(Logic::ResourcesManager& rm, Logic::TimeManager& tm) : _topBarUI(rm, tm)
	{
	}


	UIManager::~UIManager()
	{
	}


	void UIManager::init()
	{
		// Cargamos la ventana que muestra la barra superior
		


		_uiWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UI.layout");

		_topBarUI.init();

		_sideBarUI.init();

		_uiWindow->addChild(_topBarUI.getResourcesWindow());
		_uiWindow->addChild(_sideBarUI.getSideBarWindow());

		
	}

	void UIManager::activate()
	{
		// Activamos la interfaz de usuario
		
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uiWindow);


		_topBarUI.activate();
		_sideBarUI.activate();
		_uiWindow->setVisible(true);
		_uiWindow->activate();

	}

	void UIManager::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		
		_uiWindow->deactivate();
		_uiWindow->setVisible(false);
		_topBarUI.deactivate();
		_sideBarUI.deactivate();

	}

	void UIManager::tick(unsigned int msecs)
	{
		_topBarUI.tick(msecs);
		_sideBarUI.tick(msecs);
	}



}