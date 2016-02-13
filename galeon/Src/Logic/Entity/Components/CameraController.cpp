#include "CameraController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"


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
		return true;
	}

	void CameraController::deactivate()
	{

	}

	bool CameraController::HandleMessage(const ControlMessage& msg)
	{
		bool received = true;

		if (!msg._action.compare("moveForward"))
			moveForward();
		else if (!msg._action.compare("moveBackward"))
			moveBackward();
		else if (!msg._action.compare("stopMove"))
			stopMove();
		else if (!msg._action.compare("strafeLeft"))
			strafeLeft();
		else if (!msg._action.compare("strafeRight"))
			strafeRight();
		else if (!msg._action.compare("stopStrafe"))
			stopStrafe();
		else
			received = false;

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
				direction = Math::getDirection(_entity->getYaw());
				if (_movingBackward)
					direction *= -1;
			}

			if (_strafingLeft || _strafingRight)
			{
				directionStrafe =
					Math::getDirection(_entity->getYaw() + Math::PI / 2);
				if (_strafingRight)
					directionStrafe *= -1;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			Vector3 newPosition = _entity->getPosition() + direction;
			_entity->setPosition(newPosition);

			TransformMessage msg;
			msg._type = MessageType::SET_TRANSFORM;
			msg._transform = _entity->getTransform();

			msg.Dispatch(*_entity);
		}

	} // tick

} // namespace Logic