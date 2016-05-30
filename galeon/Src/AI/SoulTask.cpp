#include "SoulTask.h"

#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Entity\Entity.h"

using namespace Logic;

namespace AI
{
	Logic::BuildingType CSoulTask::getTargetBuildingType(){
		BuildingType buildingType = BuildingType::NonBuilding;

		// Chequeamos que el objetivo siga existiendo
		CEntity* targetEntity = _map->getEntityByID(_target);

		// Si lo está, vemos si es un Placeable y tiene BuildingType
		if (targetEntity != nullptr){
			CPlaceable* placeable = targetEntity->getComponent<CPlaceable>();

			if (placeable != nullptr){
				buildingType = placeable->getBuildingType();
			}
		}

		return buildingType;
	}
}