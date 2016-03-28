/**
@file PlayerController.cpp

Contiene la implementaci�n de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del rat�n y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/

#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0)
	{

	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

	} // deactivate

	//--------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key)
	{
		if(_controlledAvatar)
		{
			using namespace Logic;

			ControlMessage m;
			m._type = MessageType::CONTROL;

			switch(key.keyId)
			{
			case GUI::Key::W:
				m._action = ActionType::MOVE_FORWARD;
				break;
			case GUI::Key::S:
				m._action = ActionType::MOVE_BACKWARD;
				break;
			case GUI::Key::A:
				m._action = ActionType::STRAFE_LEFT;
				break;
			case GUI::Key::D:
				m._action = ActionType::STRAFE_RIGHT;
				break;
			default:
				return false;
			}

            m.Dispatch(*_controlledAvatar);

			return true;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			using namespace Logic;
			
			ControlMessage m;

			switch(key.keyId)
			{
			case GUI::Key::W:
			case GUI::Key::S:
				m._action = ActionType::STOP_MOVE;
				break;

			case GUI::Key::A:
			case GUI::Key::D:
				m._action = ActionType::STOP_STRAFE;
				break;

			default:
				return false;
			}

            m.Dispatch(*_controlledAvatar);

			return true;
		}

		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		/*IMPORTANTE
		  se ha comentado este código que hacía girar el personaje con el ratón ya que cancelaba
		  el uso de CEGUI impidiendo que se utilizaran sus botones
		*/

		/*
		if(_controlledAvatar)
		{
			Logic::ControlMessage m;
			m._type = Logic::MessageType::CONTROL;
			m._action = "turn";
			m._degreesMoved = -(float)mouseState.movX * TURN_FACTOR;

            m.Dispatch(*_controlledAvatar);

            return true;
		}
		*/

		return false;
	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
