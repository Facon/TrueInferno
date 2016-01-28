#include "Placeable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include <iostream>
#include <string>

namespace Logic {
	IMP_FACTORY(CPlaceable);

	CPlaceable::CPlaceable() : IComponent() {
		_tileManager = nullptr;
		_test = true;
	}

	bool CPlaceable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// TODO Remove when this check is done above
		if (entityInfo->hasAttribute("prefab") && entityInfo->getBoolAttribute("prefab"))
			return true;

		// Get TileManager's reference
		_tileManager = Logic::CTileManager::getSingletonPtr();

		// Initialiate absolute origin position
		_floorOriginPosition = 0;

		// Initialiate floor's position vector
		_floorRelativePositions = std::vector<Vector3>();

		// Get relative floor's positions. We don't know how many there are
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
		return place(entityInfo->getVector3Attribute("floor_absolute_position0"));
	} // spawn

	bool CPlaceable::accept(const TMessage &message){
		return true;
	} // accept

	void CPlaceable::process(const TMessage &message){

	} // process

	void CPlaceable::tick(unsigned int msecs){
		// TODO Eliminar
		/*if (_test){
			std::cout << this << ": 1 Current position is: x=" << _entity->getPosition().x << ", y=" << _entity->getPosition().y << ", z=" << _entity->getPosition().z << std::endl;
			getEntity()->setPosition(Vector3(rand() % 14, 4, rand() % 14), this);
			std::cout << this << ": 2 Current position is: x=" << _entity->getPosition().x << ", y=" << _entity->getPosition().y << ", z=" << _entity->getPosition().z << std::endl;
			_test = false;
		}*/

		std::cout << this << ": Current position is: x=" << _entity->getPosition().x << ", y=" << _entity->getPosition().y << ", z=" << _entity->getPosition().z << std::endl;
	} // tick

	bool CPlaceable::place(const Vector3 newOriginPosition){
		// Don't do anything if the origin position is not changing (and tiles have been already initialitated)
		if ((newOriginPosition == _floorOriginPosition) && _tiles.capacity()>0)
			return true;

		// Check if tiles allow the placement
		bool check = checkPlacementIsPossible(newOriginPosition);
		if (!check)
			return false;

		// Store new origin position
		_floorOriginPosition = newOriginPosition;

		// Clear tile vector
		_tiles.clear();

		// Store origin tile
		_tiles.push_back(_tileManager->getTile(_floorOriginPosition));

		// Get rest of tiles' positions
		for (auto it = _floorRelativePositions.cbegin(); it != _floorRelativePositions.cend(); ++it) {
			Vector3 relativePosition = (*it);

			// Store tile
			_tiles.push_back(_tileManager->getTile(_floorOriginPosition + relativePosition));
		}

		// Notify all tiles about this placeable's owner entity
		for (auto it = _tiles.begin(); it != _tiles.end(); ++it) {
			Tile* tile = (*it);
			tile->setEntityAbove(this->getEntity());
		}

		// Update entity position aligning it to the origin tile position and putting it on the tile
		// TODO Move this logic to... Entity? TileManager?
		Vector3 targetPosition = _tileManager->getTile(_floorOriginPosition)->getEntity()->getPosition() + Vector3(0, 2, 0);
		//std::cout << "Setting position for " << this->getEntity() << ": x=" << targetPosition.x << ", y=" << targetPosition.y << ", z=" << targetPosition.z << std::endl;
		_entity->setPosition(targetPosition, this);

		return true;
	}
	
	bool CPlaceable::checkPlacementIsPossible(const Vector3 &originPosition){
		// Check origin tile. If it's null or has an entity above: placement is not possible
		if (_tileManager->getTile(originPosition) == nullptr || _tileManager->getTile(originPosition)->getEntityAbove() != nullptr)
			return false;

		// Check rest of tiles' positions
		for (auto it = _floorRelativePositions.cbegin(); it != _floorRelativePositions.cend(); ++it) {
			Vector3 relativePosition = (*it);
			Tile *tile = _tileManager->getTile(originPosition + relativePosition);

			// Check tile
			if (tile == nullptr || tile->getEntityAbove() != nullptr)
				return false;
		}

		// All checks are done!
		return true;
	}

	std::string CPlaceable::getPositionAttributeName(int num){
		std::stringstream attributeName;
		attributeName << "floor_relative_position" << num;
		return attributeName.str();
	}

	const std::vector<Tile*> CPlaceable::getTiles(){
		return _tiles;
	}

} // namespace Logic