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
		_entityAbove = nullptr;
	}

	bool Tile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		assert(entityInfo->hasAttribute("position") && "Tile without 'position' attribute");

		// Read logic position
		_logicPosition = entityInfo->getVector3Attribute("position");

		// Register real (non-prefab) tiles in the TileManager
		CTileManager::getSingletonPtr()->registerTile(this);

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

	void Tile::setEntityAbove(CEntity *entityAbove){
		_entityAbove = entityAbove;
	}

	const CEntity* Tile::getEntityAbove(){
		return _entityAbove;
	}

} // namespace Logic