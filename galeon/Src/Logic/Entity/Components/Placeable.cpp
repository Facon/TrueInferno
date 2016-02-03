#include "Placeable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/Managers/BuildingManager.h"
#include <iostream>
#include <string>

namespace Logic {
	RTTI_ROOT_IMPL(CPlaceable);
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

		// Si el edificio fue creado por prefab, es posible que al hacer spawn no tenga el atributo de posici�n l�gica
		if (entityInfo->hasAttribute("floor_absolute_position0")){
			// Place placeable on its configured initial position
			if (!place(entityInfo->getVector3Attribute("floor_absolute_position0"))){
				return false;
			}

			// Get memory for the tiles: The origin and all the relatives
			_tiles = std::vector<Tile*>(_floorRelativePositions.size() + 1);
		}

		// Register building in manager
		Logic::CBuildingManager::getSingletonPtr()->registerBuilding(this);

		return true;
	} // spawn
	
	/*
	bool CPlaceable::accept(const TMessage &message){
		return true;
	} // accept

	void CPlaceable::process(const TMessage &message){

	} // process
	*/

	void CPlaceable::tick(unsigned int msecs){
	} // tick

	bool CPlaceable::place(const Vector3 newOriginPosition){
		// Don't do anything if the origin position is not changing (and tiles have been already initialitated)
		if ((newOriginPosition == _floorOriginPosition) && _tiles.capacity()>0)
			return true;

		// Check if tiles allow the placement
		bool check = checkPlacementIsPossible(newOriginPosition);
		if (!check)
			return false;

		// Clean entity above in old tiles
		_tileManager->getTile(_floorOriginPosition)->setEntityAbove(nullptr);
		for (auto it = _floorRelativePositions.cbegin(); it != _floorRelativePositions.cend(); ++it) {
			Vector3 relativePosition = (*it);
			_tileManager->getTile(_floorOriginPosition + relativePosition)->setEntityAbove(nullptr);
		}

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

		// Notify all tiles about this placeable's owner entity and calculate target position right in the center (average) of all tiles
		Vector3 targetPosition(0, 0, 0);
		for (auto it = _tiles.begin(); it != _tiles.end(); ++it) {
			Tile* tile = (*it);
			tile->setEntityAbove(this->getEntity());
			targetPosition += tile->getEntity()->getPosition();
		}
		targetPosition /= _tiles.size();

		// Move the object a bit over the tile
		targetPosition += Vector3(0, 1, 0);
		_entity->setPosition(targetPosition);

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