/**
@file AvatarController.cpp

Contiene la implementaci�n del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AvatarController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Message.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CAvatarController);
	IMP_FACTORY(CAvatarController);
	
	//---------------------------------------------------------

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CAvatarController::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::HandleMessage(const ControlMessage& msg)
	{
		bool received = true;

		switch (msg._action)
		{
		case ActionType::MOVE_FORWARD:
			walk();
			break;
		case ActionType::MOVE_BACKWARD:
			walkBack();
			break;
		case ActionType::STRAFE_LEFT:
			strafeLeft();
			break;
		case ActionType::STRAFE_RIGHT:
			strafeRight();
			break;
		case ActionType::STOP_MOVE:
			stopWalk();
			break;
		case ActionType::STOP_STRAFE:
			stopStrafe();
			break;
		case ActionType::TURN:
			turn(msg._degreesMoved);
			break;
		default:
			received = false;
			break;
		}

		return received;
	}

	void CAvatarController::turn(float amount) 
	{
		Vector3 entityRotation = _entity->getRotation();
		entityRotation.y += amount;

	} // turn
	
	//---------------------------------------------------------

	void CAvatarController::walk() 
	{
		_walking = true;

		// Cambiamos la animaci�n
		AnimationMessage msg;
		msg._type = MessageType::SET_ANIMATION;
		msg._action = "Walk";
		msg._activated = true;

		msg.Dispatch(*_entity);
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		_walkingBack = true;

		// Cambiamos la animaci�n
		AnimationMessage msg;
		msg._type = MessageType::SET_ANIMATION;
		msg._action = "WalkBack";
		msg._activated = true;

		msg.Dispatch(*_entity);
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = _walkingBack = false;

		// Cambiamos la animaci�n si no seguimos desplaz�ndonos
		// lateralmente
		if(!(_strafingLeft || _strafingRight))
		{
			AnimationMessage msg;
			msg._type = MessageType::SET_ANIMATION;
			msg._action = "Idle";
			msg._activated = true;

            msg.Dispatch(*_entity);
		}

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		_strafingLeft = true;

		// Cambiamos la animaci�n
		AnimationMessage msg;
		msg._type = MessageType::SET_ANIMATION;
		msg._action = "StrafeLeft";
		msg._activated = true;

        msg.Dispatch(*_entity);
	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		_strafingRight = true;

		// Cambiamos la animaci�n
		AnimationMessage msg;
		msg._type = MessageType::SET_ANIMATION;
		msg._action = "StrafeRight";
		msg._activated = true;

        msg.Dispatch(*_entity);
	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafe() 
	{
		_strafingLeft = _strafingRight = false;

		// Cambiamos la animaci�n si no seguimos andando
		if(!(_walking || _walkingBack))
		{
			AnimationMessage message;
			message._type = MessageType::SET_ANIMATION;
			message._action = "Idle";
			message._activated = true;

            message.Dispatch(*_entity);
		}

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la pr�xima posici�n
		// Calculamos si hay vectores de direcci�n de avance y strafe,
		// hayamos la direcci�n de la suma y escalamos seg�n la
		// velocidad y el tiempo transcurrido.
		if(_walking || _walkingBack || _strafingLeft || _strafingRight)
		{
			Vector3 direction(Vector3::ZERO);
			Vector3 directionStrafe(Vector3::ZERO);

			if (_walkingBack)
				int kk = 1;

			if(_walking || _walkingBack)
			{
				direction = Math::getDirection(_entity->getRotation().y);
				if(_walkingBack)
					direction *= -1;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
					Math::getDirection(_entity->getRotation().y + Math::PI / 2);
				if(_strafingRight)
					directionStrafe *= -1;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			// Enviar un mensaje para que el componente f�sico mueva el personaje
			//PhysicMessage message;
			//message._type = MessageType::AVATAR_WALK;
			//message._point = direction;

            //message.Dispatch(*_entity);

			Vector3 newPosition = _entity->getPosition() + direction;
			_entity->setPosition(newPosition);

			TransformMessage msg;
			msg._type = MessageType::SET_TRANSFORM;
			msg._transform = _entity->getTransform();

			msg.Dispatch(*_entity);
		}

	} // tick

} // namespace Logic
