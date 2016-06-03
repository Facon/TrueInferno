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
		_adjacentTiles = nullptr;
		_tileManager = CTileManager::getSingletonPtr();
		_placeableAbove = nullptr;
	}

	Tile::~Tile(){
		delete _adjacentTiles;
	}

	bool Tile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("position") && "Tile without 'position' attribute");

		// Read logic position
		_logicPosition = entityInfo->getVector3Attribute("position");

		// Register real (non-prefab) tiles in the TileManager
		_tileManager->registerTile(this);

		return true;
	} // spawn

	void Tile::tick(unsigned int msecs) {
	} // tick

	bool Tile::changeTerrain(const TerrainType &terrainType){
		// Determinamos el nombre del material a usar en función del enumerado
		std::string materialName;

		switch (terrainType) {
		case TerrainType::Empty:
			materialName = "Terrain/Empty";
			break;

		case TerrainType::Mineral:
			materialName = "Terrain/Mineral";
			break;

		case TerrainType::Gas:
			materialName = "Terrain/Gas";
			break;

		default:
			assert(false && "Unknown terrain type");
			return false;
		}

		// Actualizamos el enumerado
		_terrainType = terrainType;

		// Alternative material for odd tiles
		/*
		std::string materialName(baseMaterialName);
		materialName += terrainTypeChar;

		if ((x + z) % 2 != 0)
		materialName = materialName + "_alt";
		*/

		// Cambiamos el material
		MaterialMessage message(materialName);

		bool result = message.Dispatch(*_entity);
		assert(result && "Can't change terrain material");

		return result;
	}

	const Vector3 Tile::getLogicPosition() {
		return _logicPosition;
	}

	void Tile::setPlaceableAbove(CPlaceable *placeableAbove){
		_placeableAbove = placeableAbove;
	}

	const CPlaceable* Tile::getPlaceableAbove() const{
		return _placeableAbove;
	}

	bool Tile::allowsPlaceable(const CPlaceable *placeable) const{
		// Si hay algún otro Placeable o si el terreno es de obstáculo fijo
		if (_placeableAbove != nullptr)
			return false;

		return (placeable->canBePlacedOnTerrain(_terrainType));
	}

	const std::vector<Tile*>* Tile::getAdjacentTiles(){
		// Si no ha sido inicializado todavía
		if (_adjacentTiles == nullptr){
			_adjacentTiles = new std::vector<Tile*>();

			Tile* adjacent = _tileManager->getTile(_logicPosition + Vector3(-1, 0, 0));
			if (adjacent != nullptr)
				_adjacentTiles->push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(1, 0, 0));
			if (adjacent != nullptr)
				_adjacentTiles->push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(0, 0, -1));
			if (adjacent != nullptr)
				_adjacentTiles->push_back(adjacent);
			adjacent = _tileManager->getTile(_logicPosition + Vector3(0, 0, 1));
			if (adjacent != nullptr)
				_adjacentTiles->push_back(adjacent);
		}

		return _adjacentTiles;
	}

	bool Tile::canPassSoulPath(){
		// Si el terreno no está vacío la SoulPath no puede pasar
		if (_terrainType != Empty)
			return false;

		// Si no hay ningún placeable encima, la SoulPath puede atravesar
		if (_placeableAbove == nullptr)
			return true;

		// En otro caso delegamos en el Placeable
		return _placeableAbove->canPassSoulPath();
	}

	bool Tile::canPassWalkingSoul(){
		// Si no hay ningún placeable encima, las almas no pueden caminar
		if (_placeableAbove == nullptr)
			return false;

		// En otro caso delegamos en el Placeable
		return _placeableAbove->canPassWalkingSoul();
	}

	void Tile::printDebugInfo(){
		std::cout << "pos=" << _logicPosition << ", terrain=" << _terrainType << ", placeable=" << _placeableAbove << ", passSoulPath=" << canPassSoulPath() << ", passWalkingSoul=" << canPassWalkingSoul();
	}


} // namespace Logic