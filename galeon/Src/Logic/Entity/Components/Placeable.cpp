#include "Placeable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Managers/TileManager.h"
#include <iostream>
#include <string>

namespace Logic {
	IMP_FACTORY(Placeable);

	Placeable::Placeable() : IComponent() {
	}

	bool Placeable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// TODO Remove when this check is done above
		if (entityInfo->hasAttribute("prefab") && entityInfo->getBoolAttribute("prefab"))
			return true;

		// Get TileManager's reference
		CTileManager* tileManager = Logic::CTileManager::getSingletonPtr();

		// Get floor's absolute initial position
		_floorOriginPosition = entityInfo->getVector3Attribute("floor_absolute_position0");

		// Initialiate floor's position vector
		_floorRelativePositions = std::vector<Vector3>();

		// Get relative floor's positions
		int numPosition = 1;
		std::string attributeName = getPositionAttributeName(numPosition);
		while (entityInfo->hasAttribute(attributeName)){
			Vector3 floorRelativePosition = entityInfo->getVector3Attribute(attributeName);

			// Store floor's position
			_floorRelativePositions.push_back(floorRelativePosition);

			attributeName = getPositionAttributeName(++numPosition);
		}

		// Get memory for the tiles: The origin and all the relatives
		_tiles = std::vector<Tile*>(_floorRelativePositions.size()+1);

		// Place placeable on its configured initial position
		return place(_floorOriginPosition);

		return true;
	} // spawn

	bool Placeable::accept(const TMessage &message){
		return true;
	} // accept

	void Placeable::process(const TMessage &message){

	} // process

	void Placeable::tick(unsigned int msecs){
	} // tick

	bool Placeable::place(const Vector3 logicPosition){
		// Store origin tile
		_tiles.push_back(Logic::CTileManager::getSingletonPtr()->getTile(_floorOriginPosition));

		// Get rest of tiles' positions
		for (auto it = _floorRelativePositions.cbegin(); it != _floorRelativePositions.cend(); ++it) {
			Vector3 relativePosition = (*it);

			// Store tile
			_tiles.push_back(Logic::CTileManager::getSingletonPtr()->getTile(_floorOriginPosition + relativePosition));
		}

		// TODO Check tiles!
		return true;
	}
	
	std::string Placeable::getPositionAttributeName(int num){
		std::stringstream attributeName;
		attributeName << "floor_relative_position" << num;
		return attributeName.str();
	}

	const std::vector<Tile*> Placeable::getTiles(){
		return _tiles;
	}

} // namespace Logic