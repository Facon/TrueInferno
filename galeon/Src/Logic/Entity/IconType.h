#ifndef ICON_TYPE_H_
#define ICON_TYPE_H_

#include "Logic/ResourcesManager.h"
#include "BuildingType.h"
#include "Components\Toggleable.h"

namespace Logic 
{
	namespace IconType
	{
		// TODO Faltan los iconos comentados
		enum IconType
		{
			NONE = 0,
			OK = 1,
			BURNING, // WTF?
			SHOVEL,
			CLOSE,
			COKE,
			CRUDE,
			NOT_OK,
			FLAMES,
			SOUL_PATH,
			EVILWORKS,
			FURNACE,
			GAS_PLANT,
			HELLQUARTERS,
			MINE,
			CROSS_SWORD, // WTF?
			SOUL, // WTF?
			POWER_GENERATOR,
			PURE_EVIL,
			REFINED,
			REFINERY,
			REPAIR,
			RESEARCH_LABS,
			POWER_ON,
			CLOCK,
			JUDGEMENT,
			COGS, // WTF?
			WAREHOUSE,
			//GAS,
			//MINERAL,
			//EVILATOR,
		};

		// Conversor de ResourceType a IconType
		static IconType resourceType2IconType(ResourceType resourceType){
			switch (resourceType){
			case ResourceType::COKE:
				return IconType::COKE;

			case ResourceType::CRUDE:
				return IconType::CRUDE;

			case ResourceType::GAS:
				//return IconType::GAS;
				// TODO Solución temporal hasta que estén los iconos que faltan
				return IconType::GAS_PLANT;

			case ResourceType::MINERAL:
				//return IconType::MINERAL;
				// TODO Solución temporal hasta que estén los iconos que faltan
				return IconType::MINE;

			case ResourceType::PURE_EVIL:
				return IconType::PURE_EVIL;

			case ResourceType::REFINED:
				return IconType::REFINED;

			default:
				return IconType::NONE;
			}
		}

		// Conversor de BuildingType a IconType
		static IconType buildingType2IconType(BuildingType buildingType){
			switch (buildingType){
			case BuildingType::Evilator:
				//return IconType::EVILATOR;
				// TODO Solución temporal hasta que estén los iconos que faltan
				return IconType::CLOCK;

			case BuildingType::EvilWorks:
				return IconType::EVILWORKS;

			case BuildingType::Furnace:
				return IconType::FURNACE;

			case BuildingType::GasPlant:
				return IconType::GAS_PLANT;

			case BuildingType::HellQuarters:
				return IconType::HELLQUARTERS;

			case BuildingType::Mine:
				return IconType::MINE;

			case BuildingType::PowerGenerator:
				return IconType::POWER_GENERATOR;

			case BuildingType::Refinery:
				return IconType::REFINERY;

			case BuildingType::ResearchLabs:
				return IconType::RESEARCH_LABS;

			case BuildingType::Warehouse:
				return IconType::WAREHOUSE;

			default:
				return IconType::NONE;
			}
		}

		// Conversor de LogicRequirement a IconType
		static IconType logicRequirement2IconType(LogicRequirement logicRequirement){
			switch (logicRequirement){
			case LogicRequirement::Workers:
				return IconType::SOUL;

			case LogicRequirement::Energy:
				return IconType::POWER_GENERATOR;

			case LogicRequirement::Player:
				return IconType::REPAIR;

			default:
				return IconType::NONE;
			}
		}
	} // namespace IconType
}

#endif