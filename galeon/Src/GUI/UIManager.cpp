#include "UIManager.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

namespace GUI
{
	UIManager::UIManager(Logic::ResourcesManager& rm, Logic::HFManager& hfm, Logic::TimeManager& tm) : _topBarUI(rm, hfm, tm)
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
		_eventUI.init();
		_buildingSelectionUI.init();

	}

	void UIManager::release()
	{
		// Releasing memory

		_topBarUI.release();
		_sideBarUI.release();
		_eventUI.release();
		_buildingSelectionUI.release();
		_uiWindow->destroy();
	}

	void UIManager::activate()
	{
		// Activamos la interfaz de usuario
		
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uiWindow);
		_topBarUI.activate();
		_sideBarUI.activate();
		_eventUI.activate();
		_buildingSelectionUI.activate();
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
		_buildingSelectionUI.deactivate();
		_eventUI.deactivate();

	}

	void UIManager::tick(unsigned int msecs)
	{
		_topBarUI.tick(msecs);
		_sideBarUI.tick(msecs);
		_eventUI.tick(msecs);
		_buildingSelectionUI.tick(msecs);
	}



}