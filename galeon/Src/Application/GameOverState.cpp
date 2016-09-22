//---------------------------------------------------------------------------
// GameOverState.cpp
//---------------------------------------------------------------------------

/**
@file GameOverState.cpp

Contiene la implementaci�n del estado de game over.

@see Application::CApplicationState
@see Application::CGameOverState

@author David Llans�
@date Agosto, 2010
*/

#include "GameOverState.h"

#include <iostream>

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/widgets/PushButton.h>

#include "GUI/Server.h"
#include "Logic\Maps\Managers\GameManager.h"

namespace Application {

	CGameOverState::~CGameOverState() 
	{
	} // ~CGameOverState

	//--------------------------------------------------------

	bool CGameOverState::init() 
	{
		CApplicationState::init();

		_victoryWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Victory.layout");

		_victoryWindow->getChildElement("Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&CGameOverState::exitReleased, this));

        _gameOverWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Defeat.layout");

        _gameOverWindow->getChildElement("Exit")->
                subscribeEvent(CEGUI::PushButton::EventClicked,
                               CEGUI::SubscriberSlot(&CGameOverState::exitReleased, this));

		return true;

	} // init

	//--------------------------------------------------------

	void CGameOverState::release() 
	{
		CApplicationState::release();
		_victoryWindow->getChildElement("Exit")->removeAllEvents();
		_gameOverWindow->getChildElement("Exit")->removeAllEvents();

	} // release

	//--------------------------------------------------------

	void CGameOverState::activate() 
	{
		CApplicationState::activate();

		// Victoria? Panel de derrota por defecto
		bool victory = Logic::CGameManager::getSingletonPtr()->getVictory();

		if (victory)
		{
			// Activamos la ventana de victoria
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_victoryWindow);
			_victoryWindow->setVisible(true);
			_victoryWindow->activate();
		}
		else
		{
			// Activamos la ventana de derrota
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_gameOverWindow);
			_gameOverWindow->setVisible(true);
			_gameOverWindow->activate();
		}

		// Activamos el ratón
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

	} // activate

	//--------------------------------------------------------

	void CGameOverState::deactivate() 
	{
        CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();

		_victoryWindow->deactivate();
		_victoryWindow->setVisible(false);

        _gameOverWindow->deactivate();
        _gameOverWindow->setVisible(false);

        CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameOverState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameOverState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameOverState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameOverState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameOverState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameOverState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------

	bool CGameOverState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menu");
		return true;

	} // exitReleased

} // namespace Application
