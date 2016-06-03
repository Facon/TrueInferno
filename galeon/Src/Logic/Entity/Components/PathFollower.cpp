#include "PathFollower.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/SoulManager.h"
#include "Logic/TimeManager.h"
#include "Map/MapEntity.h"
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
		if (!_targetReached && _path.empty() && (_entity->getPosition().squaredDistance(_targetPosition) <= ZERO_DISTANCE))
		{
			//std::cout << "Worker reached his destination!" << std::endl;
			_targetReached = true;
		}

		// Si hemos llegado pero todavía no hemos podido notificarlo
		if (_targetReached && !_targetReachedNotified){
			// Enviamos la notificación de que hemos llegado
			WalkSoulPathMessage m(MessageType::WALK_SOUL_PATH_FINISHED);
			_targetReachedNotified = m.Dispatch(*_entity);
		}

		if (!_moving && !_path.empty())
		{
			_moving = true;

			Vector3 entityPosition(_entity->getPosition());
			_startPosition = entityPosition;
			do{
				Vector3 newPos(_path.front());
				_path.pop();
				_targetPosition = Vector3(newPos.x, entityPosition.y, newPos.z);
			} while (_path.size()>0 && _startPosition.squaredDistance(_targetPosition) <= ZERO_DISTANCE);
			
			_dir = _targetPosition - _startPosition;
			_dir.normalise();
		}

		if (_moving)
		{
			// Espacio recorrido = v * t en la dirección normalizada
			Vector3 position = _entity->getPosition();
			float baseSpeed = 1.0f / 1000; // 1 ud./s = 1 ud. / 1000 ms
			float positionIncrement = baseSpeed * CSoulManager::getSingletonPtr()->getMovementSpeed() * msecs;

			// Si nos pasamos (pico de FPS o por breakpoint en debug) ajustamos a la distancia que quedaba
			// Chequeamos con los valores al cuadrado
			float remainingSqDist = _entity->getPosition().squaredDistance(_targetPosition);
			if (remainingSqDist <= positionIncrement*positionIncrement){
				// Operamos con los valores normales
				positionIncrement = sqrt(remainingSqDist);
				_moving = false;
			}

			position += _dir * positionIncrement;

			// Actualizamos posición
			_entity->setPosition(position);
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