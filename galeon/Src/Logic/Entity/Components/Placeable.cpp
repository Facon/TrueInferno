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
		_tiles.clear();
	}

	bool CPlaceable::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		// TODO Remove when this check is done above
		if (entityInfo->hasAttribute("prefab") && entityInfo->getBoolAttribute("prefab"))
			return true;

		// Get TileManager's reference
		_tileManager = Logic::CTileManager::getSingletonPtr();

		// Initialiate absolute origin position
		_floorOriginPosition = 0;

		// Read floor's size
		_floorX = entityInfo->getIntAttribute("floor_x");
		_floorZ = entityInfo->getIntAttribute("floor_z");

		// Si el edificio fue creado por prefab, es posible que al hacer spawn no tenga el atributo de posición lógica
		if (entityInfo->hasAttribute("floor_absolute_position0")){
			// Place placeable on its configured initial position
			if (!place(entityInfo->getVector3Attribute("floor_absolute_position0"))){
				return false;
			}

			// Get memory for all the tiles
			_tiles = std::vector<Tile*>(_floorX * _floorZ);
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
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));
				tile->setEntityAbove(nullptr);
			}
		}

		// Store new origin position
		_floorOriginPosition = newOriginPosition;

		// Clear tile vector
		_tiles.clear();

		// Initialize vector to calculate the average position of all tiles
		Vector3 centerPosition(0, 0, 0);

		// For each tile
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));

				// Store it internally
				_tiles.push_back(tile);

				// Notify its new entity above
				tile->setEntityAbove(this->getEntity());

				// Add it to the average position
				centerPosition += tile->getEntity()->getPosition();
			}
		}

		// Get the average position
		centerPosition /= _tiles.size();

		// Add some height to the average found so the object is over the tile
		centerPosition += Vector3(0, HEIGHT_OVER_TILE, 0);

		// Move entity
		_entity->setPosition(centerPosition);

		return true;
	}
	
	bool CPlaceable::checkPlacementIsPossible(const Vector3 &checkPosition){
		// Check origin tile. If it's null or has an entity above: placement is not possible
		if (_tileManager->getTile(checkPosition) == nullptr || _tileManager->getTile(checkPosition)->getEntityAbove() != nullptr)
			return false;

		// For each tile
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(checkPosition + Vector3(x, 0, z));

				// Check tile
				if (tile == nullptr || tile->getEntityAbove() != nullptr)
					return false;
			}
		}

		// All checks are done!
		return true;
	}

	const std::vector<Tile*> CPlaceable::getTiles(){
		return _tiles;
	}

} // namespace Logic