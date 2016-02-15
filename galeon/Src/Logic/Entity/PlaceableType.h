#ifndef PLACEABLE_TYPE_H_
#define PLACEABLE_TYPE_H_

#include <string>
#include <cassert>

namespace Logic {
	enum PlaceableType {
		Obstacle,
		Building,
		SoulPath,
		NonPlaceable
	};

} // namespace Logic

#endif // PLACEABLE_TYPE_H_