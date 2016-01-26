/*
#include "Tile.h"

#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	IMP_FACTORY(Tile);

	Tile::Tile() : IComponent() {

	}

	bool Tile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		std::cout << "Tile spawned!" << std::endl;
		return true;
	} // spawn

	bool Tile::accept(const TMessage &message){
		return true;
	} // accept

	void Tile::process(const TMessage &message){

	} // process

	void Tile::tick(unsigned int msecs){

	} // tick

} // namespace Logic
*/