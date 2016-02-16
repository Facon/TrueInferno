#include "HellQuarters.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CHellQuarters);
	IMP_FACTORY(CHellQuarters);

	CHellQuarters::CHellQuarters() : IComponent() {
	}

	bool CHellQuarters::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		_timeSinceLastSpawn = 0;
		
		assert(entityInfo->hasAttribute("numInitialSouls") && "numInitialSouls is not defined");
		_numAvailableSouls = entityInfo->getIntAttribute("numInitialSouls");
		
		assert(entityInfo->hasAttribute("secondsBetweenSpawns") && "secondsBetweenSpawns is not defined");
		_timeBetweenSpawns = 1000 * entityInfo->getIntAttribute("secondsBetweenSpawns");

		assert(entityInfo->hasAttribute("numSpawnedSouls") && "numSpawnedSouls is not defined");
		_numSpawnedSouls = entityInfo->getIntAttribute("numSpawnedSouls");

		_soulsToWorkRequest = false;
		_soulsToBurnRequest = false;

		return true;
	} // spawn

	void CHellQuarters::tick(unsigned int msecs){
		spawnSouls(msecs);
	} // tick

	void CHellQuarters::spawnSouls(unsigned int msecs){
		// Chequeamos si ha pasado tiempo suficiente
		_timeSinceLastSpawn += msecs;
		if (_timeSinceLastSpawn >= _timeBetweenSpawns){
			_timeSinceLastSpawn -= _timeBetweenSpawns;
			_numAvailableSouls += _numSpawnedSouls;

			std::cout << "available souls = " << _numAvailableSouls << std::endl;

			// TODO ¿Reproducimos algún sonido o animación de almas nuevas?
		}
	}

} // namespace Logic