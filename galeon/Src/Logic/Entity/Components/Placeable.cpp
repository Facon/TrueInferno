#include "Placeable.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/Managers/BuildingManager.h"
#include "Logic/Entity/PlaceableType.h"

#include <iostream>
#include <string>

namespace Logic {
	RTTI_ROOT_IMPL(CPlaceable);
	IMP_FACTORY(CPlaceable);

	CPlaceable::CPlaceable() : IComponent() {
		_tileManager = nullptr;
		_tiles.clear();
		_adyacentTiles.clear();
		_placeableType = NonPlaceable;
		_buildingType = NonBuilding;
	}

	CPlaceable::~CPlaceable() {
		// Si es un edificio
		if (_placeableType == Building)
			// Lo desregistramos en el manager
			Logic::CBuildingManager::getSingletonPtr()->unregisterBuilding(this);
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
		if (!entityInfo->hasAttribute("floor_x") || !entityInfo->hasAttribute("floor_z")){
			std::cout << "floor_x and floor_z must be defined" << std::endl;
			return false;
		}
		_floorX = entityInfo->getIntAttribute("floor_x");
		_floorZ = entityInfo->getIntAttribute("floor_z");

		// Determinamos el tipo de placeable que es
		_placeableType = parsePlaceableType(entityInfo->getStringAttribute("placeableType"));

		// Si el edificio fue creado por prefab, es posible que al hacer spawn no tenga el atributo de posición lógica
		if (entityInfo->hasAttribute("floor_absolute_position0")){
			// Place placeable on its configured initial position
			if (!place(entityInfo->getVector3Attribute("floor_absolute_position0"))){
				return false;
			}

			// Get memory for all the tiles
			_tiles = std::vector<Tile*>(_floorX * _floorZ);
		}

		// Si es un edificio
		if (_placeableType == Building){
			// Determinamos el tipo
			_buildingType = parseBuildingType(entityInfo->getStringAttribute("type"));

			// Lo registramos en el manager
			Logic::CBuildingManager::getSingletonPtr()->registerBuilding(this);
		}
		else
			_buildingType = NonBuilding;

		return true;
	} // spawn

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

		// Clear placeable above in old tiles
		_tileManager->getTile(_floorOriginPosition)->setPlaceableAbove(nullptr);
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));
				tile->setPlaceableAbove(nullptr);
			}
		}

		// Store new origin position
		_floorOriginPosition = newOriginPosition;

		// Clear tile vector
		_tiles.clear();
		_adyacentTiles.clear();

		// Initialize vector to calculate the average position of all tiles
		Vector3 centerPosition(0, 0, 0);

		// For each tile
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(_floorOriginPosition + Vector3(x, 0, z));

				// Store it internally
				_tiles.push_back(tile);

				// Notify its new entity above
				tile->setPlaceableAbove(this);

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

		// Update adyacent tiles
		updateAdyacentTiles();

		return true;
	}
	
	bool CPlaceable::checkPlacementIsPossible(const Vector3 &checkPosition){
		// Check origin tile. If it's null or can't place anything on it: placement is not possible
		if (_tileManager->getTile(checkPosition) == nullptr || !_tileManager->getTile(checkPosition)->canPlaceSomething())
			return false;

		// For each tile
		for (int x = 0; x < _floorX; ++x) {
			for (int z = 0; z < _floorZ; ++z) {
				Tile* tile = _tileManager->getTile(checkPosition + Vector3(x, 0, z));

				// Check tile
				if (tile == nullptr || !tile->canPlaceSomething())
					return false;
			}
		}

		// All checks are done!
		return true;
	}

	const std::vector<Tile*> CPlaceable::getTiles(){
		return _tiles;
	}

	const std::unordered_set<Tile*> CPlaceable::getAdyacentTiles(){
		return _adyacentTiles;
	}

	bool CPlaceable::canPassSoulPath(){
		// Sólo permitimos pasar SoulPath si hay SoulPath
		return _placeableType == SoulPath;
	}

	bool CPlaceable::canPassWalkingSoul(){
		// Sólo permitimos a las almas caminar si hay SoulPath. Podría interesar dejarlas atravesar también los edificios
		return _placeableType == SoulPath;
	}

	bool CPlaceable::isBuilding(){
		return _placeableType == Building;
	}

	bool CPlaceable::HandleMessage(const PlaceMessage& msg){
		return place(msg.position);
	}

	PlaceableType CPlaceable::parsePlaceableType(const std::string& name){
		if (name == "Obstacle"){
			return Obstacle;
		}
		else if (name == "Building"){
			return Building;
		}
		else if (name == "SoulPath"){
			return SoulPath;
		}
		else{
			assert("PlaceableType name unknown");
			// TODO lanzar excepción en vez de assert y eliminar valor NoPlaceable
			return NonPlaceable;
		}
	}

	BuildingType CPlaceable::parseBuildingType(const std::string& name){
		if (name == "HellQuarters"){
			return HellQuarters;
		}
		else if (name == "Furnace"){
			return Furnace;
		}
		else if (name == "Evilworks"){
			return Evilworks;
		}
		else if (name == "Refinery"){
			return Refinery;
		}
		else if (name == "Evilator"){
			return Evilator;
		}
		else if (name == "Mine"){
			return Mine;
		}
		else if (name == "GasPlant"){
			return GasPlant;
		}
		else if (name == "ResearchLabs"){
			return ResearchLabs;
		}
		else if (name == "PowerGenerator"){
			return PowerGenerator;
		}
		else if (name == "Warehouse"){
			return Warehouse;
		}
		else{
			assert("BuildingType name unknown");
			// TODO lanzar excepción en vez de assert
			return NonBuilding;
		}
	}

	void CPlaceable::updateAdyacentTiles(){
		// For each tile
		for (auto itTiles = _tiles.cbegin(); itTiles != _tiles.cend(); ++itTiles){
			Tile *tile = (*itTiles);

			// For each of its adyacent tiles
			for (auto itAdyacent = tile->getAdjacentTiles()->cbegin(); itAdyacent != tile->getAdjacentTiles()->cend(); ++itAdyacent){
				Tile *adyacentTile = (*itAdyacent);

				// Save adyacent tile (* which may belong to the Placeable's tile. Read below)
				_adyacentTiles.insert(*itAdyacent);
			}
		}

		// As a tile is adyacent to the placeable's tiles we have to remove them
		// (* Here we check the set once for each of the placeable's tiles. This way we avoid checking the tile vector for each tile-adyacent)
		for (auto itTiles = _tiles.cbegin(); itTiles != _tiles.cend(); ++itTiles){
			Tile *tile = (*itTiles);
			_adyacentTiles.erase(tile);
		}
	}

} // namespace Logic