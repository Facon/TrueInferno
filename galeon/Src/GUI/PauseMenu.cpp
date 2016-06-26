#include "PauseMenu.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

namespace GUI
{
	PauseMenu::PauseMenu()
	{
	}

	PauseMenu::~PauseMenu()
	{
	}

	void PauseMenu::init()
	{
		// Cargamos la ventana que muestra la barra superior
		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Pause.layout");
		_menuWindow->getChildElement("ButtonFrame/ExitButton")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&PauseMenu::exitReleased, this));
	}

	void PauseMenu::release()
	{
		_menuWindow->getChildElement("ButtonFrame/ExitButton")->removeAllEvents();
	}

	bool PauseMenu::exitReleased(const CEGUI::EventArgs& e)
	{
		_exit = true;
		return true;
	}

	void PauseMenu::activate()
	{
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_menuWindow);
		// Activamos la interfaz de usuario
		_menuWindow->setVisible(true);
		_menuWindow->activate();
	}

	void PauseMenu::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
	}

	void PauseMenu::tick(unsigned int msecs)
	{
		
	}

	void PauseMenu::open()
	{
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_menuWindow);
		_menuWindow->setVisible(true);
	}

	void PauseMenu::close()
	{
		_menuWindow->setVisible(false);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_menuWindow);
	}

}