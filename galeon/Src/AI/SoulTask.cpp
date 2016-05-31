#include "SoulTask.h"

#include "Logic\Entity\Components\Placeable.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/Billboard.h"

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

	bool CSoulTask::addDestinationBuildingIcon(){
		// Obtenemos la entidad que va a ejecutar la tarea
		CEntity* executor = _map->getEntityByID(_executorId);

		// Determinamos el tipo de edificio al que va
		BuildingType buildingType = getTargetBuildingType();

		if (buildingType != BuildingType::NonBuilding && buildingType != BuildingType::Unassigned){
			// Icono de edificio destino
			IconMessage m2(MessageType::ICON_ADD, Billboard::getBuildingIcon(buildingType));
			return m2.Dispatch(*executor);
		}

		else {
			assert(false && "Can't set icon for a destination building without buildingType");
		}

	}
}