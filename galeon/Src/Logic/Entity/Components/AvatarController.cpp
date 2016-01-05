/**
@file AvatarController.cpp

Contiene la implementaci�n del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "AvatarController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


namespace Logic 
{
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

	bool CAvatarController::accept(const TMessage &message)
	{
		return message._type == Message::CONTROL;

	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::CONTROL:
			if(!message._string.compare("walk"))
				walk();
			else if(!message._string.compare("walkBack"))
				walkBack();
			else if(!message._string.compare("stopWalk"))
				stopWalk();
			else if(!message._string.compare("strafeLeft"))
				strafeLeft();
			else if(!message._string.compare("strafeRight"))
				strafeRight();
			else if(!message._string.compare("stopStrafe"))
				stopStrafe();
			else if(!message._string.compare("turn"))
				turn(message._float);
		}

	} // process
	
	//---------------------------------------------------------

	void CAvatarController::turn(float amount) 
	{
		_entity->yaw(amount);

	} // turn
	
	//---------------------------------------------------------

	void CAvatarController::walk() 
	{
		_walking = true;

		// Cambiamos la animaci�n
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "Walk";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		_walkingBack = true;

		// Cambiamos la animaci�n
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "WalkBack";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = _walkingBack = false;

		// Cambiamos la animaci�n si no seguimos desplaz�ndonos
		// lateralmente
		if(!(_strafingLeft || _strafingRight))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		_strafingLeft = true;

		// Cambiamos la animaci�n
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "StrafeLeft";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		_strafingRight = true;

		// Cambiamos la animaci�n
		TMessage message;
		message._type = Message::SET_ANIMATION;
		message._string = "StrafeRight";
		message._bool = true;
		_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafe() 
	{
		_strafingLeft = _strafingRight = false;

		// Cambiamos la animaci�n si no seguimos andando
		if(!(_walking || _walkingBack))
		{
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
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
				direction = Math::getDirection(_entity->getYaw());
				if(_walkingBack)
					direction *= -1;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
						Math::getDirection(_entity->getYaw() + Math::PI/2);
				if(_strafingRight)
					directionStrafe *= -1;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			// Enviar un mensaje para que el componente f�sico mueva el personaje
			TMessage message;
			message._type = Message::AVATAR_WALK;
			message._vector3 = direction;
			_entity->emitMessage(message);

			//Vector3 newPosition = _entity->getPosition() + direction;
			//_entity->setPosition(newPosition);
		}

	} // tick

} // namespace Logic

