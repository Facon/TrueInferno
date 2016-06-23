#include "CameraController.h"

#include "Logic/Entity/Message.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CameraController);
	IMP_FACTORY(CameraController);

	bool CameraController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		if (entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");

		return true;

	}

	bool CameraController::activate()
	{
		CServer::getSingletonPtr()->setPlayer(_entity);
		GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(_entity);
		
		return true;
	}

	void CameraController::deactivate()
	{
		GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(nullptr);
		CServer::getSingletonPtr()->setPlayer(nullptr);
	}

	bool CameraController::HandleMessage(const ControlMessage& msg)
	{
		bool received = true;

		switch (msg._action)
		{
		case ActionType::MOVE_FORWARD:
			moveForward();
			break;
		case ActionType::MOVE_BACKWARD:
			moveBackward();
			break;
		case ActionType::STRAFE_LEFT:
			strafeLeft();
			break;
		case ActionType::STRAFE_RIGHT:
			strafeRight();
			break;
		case ActionType::STOP_MOVE:
			stopMove();
			break;
		case ActionType::STOP_STRAFE:
			stopStrafe();
			break;
		default:
			received = false;
			break;
		}

		return received;
	}

	void CameraController::moveForward()
	{
		_movingForward = true;
	}

	void CameraController::moveBackward()
	{
		_movingBackward = true;
	}

	void CameraController::stopMove()
	{
		_movingForward = _movingBackward = false;
	}

	void CameraController::strafeLeft()
	{
		_strafingLeft = true;
	}

	void CameraController::strafeRight()
	{
		_strafingRight = true;
	}

	void CameraController::stopStrafe()
	{
		_strafingLeft = _strafingRight = false;
	}

	void CameraController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (_movingForward || _movingBackward || _strafingLeft || _strafingRight)
		{
			Vector3 direction(Vector3::ZERO);
			Vector3 directionStrafe(Vector3::ZERO);

			if (_movingBackward)
				int kk = 1;

			if (_movingForward || _movingBackward)
			{
				direction = Math::getDirection(_entity->getRotation().y);
				if (_movingBackward)
					direction *= -1;

				// #perspectiva Movimiento en perspectiva isométrica. Comentar para
				// volver a movimiento ortogonal
				direction.x = direction.z;
			}

			if (_strafingLeft || _strafingRight)
			{
				directionStrafe =
					Math::getDirection(_entity->getRotation().y + Math::PI / 2);
				if (_strafingRight)
					directionStrafe *= -1;

				// #perspectiva Movimiento en perspectiva isométrica. Comentar para
				// volver a movimiento ortogonal
				directionStrafe.z = -directionStrafe.x;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			Vector3 newPosition = _entity->getPosition() + direction;
			_entity->setPosition(newPosition);

			TransformMessage msg(_entity->getTransform());

			msg.Dispatch(*_entity);
		}

	} // tick

} // namespace Logic