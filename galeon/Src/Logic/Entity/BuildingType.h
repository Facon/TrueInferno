#ifndef BUILDING_TYPE_H_
#define BUILDING_TYPE_H_

#include <string>
#include <cassert>

namespace Logic {

	/**
	Tipos de edificio.
	*/
	enum BuildingType {
		HellQuarters,
		Furnace,
		EvilWorks,
		Refinery,
		Evilator,
		Mine,
		GasPlant,
		ResearchLabs,
		PowerGenerator,
		Warehouse,
		NonBuilding,
		Unassigned
	};

	static void test(){
		int i = 0;
	}

} // namespace Logic

#endif // BUILDING_TYPE_H_