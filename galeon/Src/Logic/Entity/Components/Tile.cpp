#include "Tile.h"

#include "Logic/Entity/Entity.h"
#include <iostream>
#include "Map/MapEntity.h"

#include "Logic/Maps/Managers/TileManager.h"

#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(Tile);
	IMP_FACTORY(Tile);

	Tile::Tile() : IComponent() {
		_terrainType = TerrainType::Empty;
		_logicPosition = Vector3::ZERO;

		// Inicializamos el tama�o del vector de adyacentes
		_adjacentTiles = std::vector<Tile*>(NUM_ADJACENT);

		_tileManager = CTileManager::getSingletonPtr();
		_placeableAbove = nullptr;
	}

	bool Tile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		assert(entityInfo->hasAttribute("position") && "Tile without 'position' attribute");

		// Read logic position
		_logicPosition = entityInfo->getVector3Attribute("position");

		// Register real (non-prefab) tiles in the TileManager
		_tileManager->registerTile(this);

		return true;
	} // spawn

	/*
	bool Tile::accept(const TMessage &message){
		return true;
	} // accept
	
	void Tile::process(const TMessage &message) {

	} // process
	*/

	void Tile::tick(unsigned int msecs) {
	} // tick

	void Tile::setTerrainType(const TerrainType &terrainType){
		_terrainType = terrainType;
	}

	const TerrainType Tile::getTerrainType() {
		return _terrainType;
	}

	const Vector3 Tile::getLogicPosition() {
		return _logicPosition;
	}

	void Tile::setPlaceableAbove(CPlaceable *placeableAbove){
		_placeableAbove = placeableAbove;
	}

	const CPlaceable* Tile::getPlaceableAbove(){
		return _placeableAbove;
	}

	bool Tile::canPlaceSomething(){
		// True si no hay placeable encima
		return _placeableAbove == nullptr;
	}

	const std::vector<Tile*> Tile::getAdjacentTiles(){
		// Si no ha sido inicializado todav�a
		if (_adjacentTiles.size() == 0){
			Tile* adjacent = _tileManager->getTile(_logicPosition + Vector3(-1, 0, 0));
			if (adjacent != nullptr)
				_adjacentTiles.push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(1, 0, 0));
			if (adjacent != nullptr)
				_adjacentTiles.push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(0, 0, -1));
			if (adjacent != nullptr)
				_adjacentTiles.push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(0, 0, 1));
			if (adjacent != nullptr)
				_adjacentTiles.push_back(adjacent);
		}

		return _adjacentTiles;
	}

	bool Tile::canBuildSoulPath(){
		if (_entityAbove)
	}

	bool Tile::canBuildBuilding(){
		return (_entityAbove == nullptr);
	}

} // namespace Logic