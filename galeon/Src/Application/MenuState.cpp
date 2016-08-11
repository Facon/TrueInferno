//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "MenuState.h"

#include "Audio/Server.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "GUI/Server.h"

#include <CEGUI/CEGUI.h>
#include <iostream>

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UI.layout");
		_menuMainWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Menu.layout");
		_menuCreditsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Credits.layout");
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_menuMainWindow->getChildElement("ButtonFrame/StartButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		//_menuWindow->getChildElement("Settings")->
			//subscribeEvent(CEGUI::PushButton::EventClicked,
				//CEGUI::SubscriberSlot(&CMenuState::settingsReleased, this));

		_menuMainWindow->getChildElement("ButtonFrame/CreditsButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&CMenuState::creditsReleased, this));
		
		_menuMainWindow->getChildElement("ButtonFrame/ExitButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

		// Fallan al liberar (saliendo del juego a través del menú con Exit->Exit): No descomentar
		_menuWindow->getChildElement("ButtonFrame/StartButton")->removeAllEvents();
		_menuWindow->getChildElement("ButtonFrame/CreditsButton")->removeAllEvents();
		_menuWindow->getChildElement("ButtonFrame/ExitButton")->removeAllEvents();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_menuMainWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		// Reproducimos la música del menú
		Audio::CServer::getSingletonPtr()->playBgSound("audio_menu");

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		Logic::CServer::getSingletonPtr()->getMap()->tick(0);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			if (_onMenu)
			_app->exitRequest();
			else{
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_menuCreditsWindow);
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_menuMainWindow);
			}
			break;

		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
	
	//--------------------------------------------------------
	
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		// Reproducimos la música inicial del juego
		Audio::CServer::getSingletonPtr()->playBgSound("audio_game_1");

		_app->setState("game");
		return true;

	} // startReleased
	
	//--------------------------------------------------------

	bool CMenuState::settingsReleased(const CEGUI::EventArgs& e)
	{
		std::cout << "Settings\n";

		return true;
	}

	bool CMenuState::creditsReleased(const CEGUI::EventArgs& e)
	{
		_onMenu = false;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_menuMainWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_menuCreditsWindow);
		return true;
	}

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

} // namespace Application
