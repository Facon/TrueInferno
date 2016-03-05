#ifndef PATHFOLLOWER_H_
#define PATHFOLLOWER_H_

#include <queue>

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic
{
	class Tile;
}

namespace Logic
{
	class PathFollower : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(PathFollower);

	public:
		PathFollower() : IComponent(), _speed(0.5f), _startTime(0), _journeyLength(0.0f), 
			_moving(false), _targetReached(false), _targetReachedNotified(false),
			_startPosition(0.0f, 0.0f, 0.0f), _targetPosition(0.0f, 0.0f, 0.0f)
		{}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool HandleMessage(const WalkSoulPathMessage& msg);
		void tick(unsigned int msecs);
		void addToQueue(const std::vector<Vector3>& path);
	
	protected:
		float _speed = 0.5f;
		unsigned int _startTime;
		float _journeyLength;
		bool _moving;
		
		/** Flag a true si la entidad ha llegado al destino */
		bool _targetReached;
				
		/** Flag a true si la entidad ha podido notificar que ha llegado al destino */
		bool _targetReachedNotified;
		
		Vector3 _startPosition;
		Vector3 _targetPosition;
		//private Transform endMarker;
		std::queue<Vector3> _path;
		const float ZERO_DISTANCE = 0.001f;
	};

	REG_FACTORY(PathFollower);
}

#endif // PATHFOLLOWER_H_