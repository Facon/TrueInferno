#ifndef CAMERACONTROLLER_H_
#define CAMERACONTROLLER_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic
{
	// Check AvatarController for more info
	// CameraController is a simplified version
	class CameraController : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(CameraController);

	public:
		CameraController() : IComponent(), _movingForward(false), _movingBackward(false),
			_strafingLeft(false), _strafingRight(false), _speed(0.05f) {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual bool activate();
		virtual void deactivate();
		virtual void tick(unsigned int msecs);
		void moveForward();
		void moveBackward();
		void stopMove();
		void strafeLeft();
		void strafeRight();
		void stopStrafe();
		bool HandleMessage(const ControlMessage& msg);

	protected:
		bool _movingForward;
		bool _movingBackward;
		bool _strafingLeft;
		bool _strafingRight;
		float _speed;
	};

	REG_FACTORY(CameraController);
}

#endif // CAMERACONTROLLER_H_