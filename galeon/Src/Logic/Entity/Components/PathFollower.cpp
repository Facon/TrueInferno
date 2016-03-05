#include "PathFollower.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Application/GaleonApplication.h"
#include "BaseSubsystems/Math.h"
#include <vector>

namespace Logic
{
	RTTI_ROOT_IMPL(PathFollower);
	IMP_FACTORY(PathFollower);

	bool PathFollower::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		return true;
	}

	bool PathFollower::HandleMessage(const WalkSoulPathMessage& msg)
	{
		switch (msg._type)
		{
		case MessageType::PERFORM_WALK_SOUL_PATH:
			addToQueue(*msg._path);
			delete msg._path;
			return true;
		default:
			break;
		}
		
		return false;
	}

	void PathFollower::tick(unsigned int msecs)
	{
		if (_path.empty() && (_entity->getPosition().squaredDistance(_targetPosition) <= ZERO_DISTANCE) && !_targetReached)
		{
			std::cout << "Worker reached his destination!" << std::endl;

			// Informamos que hemos llegado
			WalkSoulPathMessage m(MessageType::WALK_SOUL_PATH_FINISHED);
			m.Dispatch(*_entity);

			//workerManager.disableWorker(this.gameObject);

			_targetReached = true;
		}

		if (!_path.empty() && !_moving)
		{
			_moving = true;

			Vector3 entityPosition(_entity->getPosition());
			_startPosition = entityPosition;
			do{
				Vector3 newPos(_path.front());
				_path.pop();
				_targetPosition = Vector3(newPos.x, entityPosition.y, newPos.z);
			} while (_path.size()>0 && _startPosition.squaredDistance(_targetPosition) <= ZERO_DISTANCE);
								
			// TODO Mirar como hacer la rotación sin usar LookAt
			//this.transform.LookAt(targetPosition);
			
			_startTime = Application::CGaleonApplication::getSingletonPtr()->getAppTime();
			_journeyLength = _startPosition.distance(_targetPosition);
		}

		if (_moving)
		{
			// Lerp
			unsigned int appTime = Application::CGaleonApplication::getSingletonPtr()->getAppTime();
			float distCovered = ((appTime - _startTime) / 1000.0f) * _speed; // / 1000.0f for secs instead of msecs
			float fracJourney = distCovered / _journeyLength;
			_entity->setPosition(Math::lerp<Vector3, float>(_startPosition, _targetPosition, fracJourney));
			// Si llega al destino, meter moving a false
			if (fracJourney >= 1.0f)
			{
				_moving = false;
			}
		}
	}

	void PathFollower::addToQueue(const std::vector<Vector3>& path)
	{
		for (auto it = path.cbegin(); it != path.cend(); ++it)
		{
			_path.push(*it);
		}
	}
}