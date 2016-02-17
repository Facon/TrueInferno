#ifndef PATHFOLLOWER_H_
#define PATHFOLLOWER_H_

#include <queue>

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
		PathFollower() : IComponent()
		{}

		//virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
	
	protected:
		std::queue<Tile*> path;
	};

	REG_FACTORY(PathFollower);
}

#endif // PATHFOLLOWER_H_