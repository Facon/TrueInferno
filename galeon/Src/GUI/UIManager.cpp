#include "UIManager.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

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
		_uiWindow->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&UIManager::HandleRootMouseDown, this));
		_topBarUI.init();
		_sideBarUI.init();
		_eventUI.init();
		_rankUI.init();
		_buildingSelectionUI.init();

	}

	bool UIManager::HandleRootMouseDown(const CEGUI::EventArgs& e)
	{
		if (e.handled==0)
			_sideBarUI.playerInteractionWithLeftClick();
		return true;
	}


	void UIManager::release()
	{
		// Releasing memory

		_topBarUI.release();
		_sideBarUI.release();
		_eventUI.release();
		_rankUI.release();
		_buildingSelectionUI.release();
		_uiWindow->removeAllEvents();
		_uiWindow->destroy();
	}

	void UIManager::activate()
	{
		// Activamos la interfaz de usuario
		
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_uiWindow);
		_topBarUI.activate();
		_sideBarUI.activate();
		_rankUI.activate();
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
		_rankUI.deactivate();
		_sideBarUI.deactivate();
		_buildingSelectionUI.deactivate();
		_eventUI.deactivate();

	}

	void UIManager::tick(unsigned int msecs)
	{
		_topBarUI.tick(msecs);
		_sideBarUI.tick(msecs);
		//_rankUI.tick(msecs);
		_eventUI.tick(msecs);
		_buildingSelectionUI.tick(msecs);
	}
}