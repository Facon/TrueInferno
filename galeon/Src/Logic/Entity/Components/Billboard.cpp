// @author Asier González

#include "Billboard.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/ResourcesManager.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/IconType.h"
#include "Logic/Entity/BuildingType.h"
#include "Logic/Entity/LogicRequirement.h"
#include "Graphics/BillboardSet.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <OgreSceneNode.h>

namespace Logic
{
	RTTI_ROOT_IMPL(Billboard);
	IMP_FACTORY(Billboard);

	const std::unordered_map<std::string, IconType::IconType> Billboard::iconTableConversor =
	{
		{ "OK", IconType::OK },
		{ "BURNING", IconType::BURNING },
		{ "SHOVEL", IconType::SHOVEL },
		{ "CLOSE", IconType::CLOSE },
		{ "COKE", IconType::COKE },
		{ "CRUDE", IconType::CRUDE },
		{ "NOT_OK", IconType::NOT_OK },
		{ "FLAMES", IconType::FLAMES },
		{ "SOUL_PATH", IconType::SOUL_PATH },
		{ "EVILWORKS", IconType::EVILWORKS },
		{ "FURNACE", IconType::FURNACE },
		{ "GAS_PLANT", IconType::GAS_PLANT },
		{ "HELLQUARTERS", IconType::HELLQUARTERS },
		{ "MINE", IconType::MINE },
		{ "CROSS_SWORD", IconType::CROSS_SWORD }, // WTF?
		{ "SOUL", IconType::SOUL },
		{ "POWER_GENERATOR", IconType::POWER_GENERATOR }, // WTF?
		{ "PURE_EVIL", IconType::PURE_EVIL },
		{ "REFINERY", IconType::REFINERY },
		{ "REPAIR", IconType::REPAIR },
		{ "RESEARCH_LABS", IconType::RESEARCH_LABS },
		{ "POWER_ON", IconType::POWER_GENERATOR },
		{ "CLOCK", IconType::CLOCK },
		{ "JUDGEMENT", IconType::JUDGEMENT },
		{ "COGS", IconType::COGS }, // WTF?
		{ "WAREHOUSE", IconType::WAREHOUSE },
		//{ "EVILATOR", IconType::EVILATOR },
		//{ "MINERAL", IconType::MINERAL },
		//{ "GAS", IconType::GAS },
		// TODO Solución temporal hasta que estén los iconos que faltan
		{ "EVILATOR", IconType::CLOCK },
		{ "MINERAL", IconType::MINE },
		{ "GAS", IconType::GAS_PLANT },
		{ "NONE", IconType::NONE},
	};
	
	const std::unordered_map<ResourceType, IconType::IconType> Billboard::resourceTableConversor =
	{
		{ NONE, IconType::NONE },
		{ MINERAL, IconType::MINE },
		{ GAS, IconType::GAS_PLANT },
		{ COKE, IconType::COKE },
		{ CRUDE, IconType::CRUDE },
		{ PURE_EVIL, IconType::PURE_EVIL },
		{ REFINED, IconType::REFINED },
		{ AETHER, IconType::NONE },
		{ HADES_FAVOR, IconType::HELLQUARTERS }
	};

	const std::unordered_map<ResourceType, IconType::IconType> Billboard::storageTableConversor =
	{
		{ NONE, IconType::NONE },
		{ MINERAL, IconType::WAREHOUSE },
		{ GAS, IconType::WAREHOUSE },
		{ COKE, IconType::POWER_GENERATOR },
		{ CRUDE, IconType::EVILWORKS },
		{ PURE_EVIL, IconType::REFINERY },
		{ REFINED, IconType::CLOCK },
		{ AETHER, IconType::RESEARCH_LABS },
		{ HADES_FAVOR, IconType::NONE }
	};

	const std::unordered_map<BuildingType, IconType::IconType> Billboard::buildingTableConversor =
	{
		{ BuildingType::EvilWorks, IconType::EVILWORKS },
		{ BuildingType::Furnace, IconType::FURNACE },
		{ BuildingType::GasPlant, IconType::GAS_PLANT },
		{ BuildingType::HellQuarters, IconType::HELLQUARTERS },
		{ BuildingType::Mine, IconType::MINE },
		{ BuildingType::PowerGenerator, IconType::POWER_GENERATOR }, // WTF?
		{ BuildingType::Refinery, IconType::REFINERY },
		{ BuildingType::ResearchLabs, IconType::RESEARCH_LABS },
		{ BuildingType::Warehouse, IconType::WAREHOUSE },
		{ BuildingType::Evilator, IconType::CLOCK },
	};

	const std::unordered_map<LogicRequirement, IconType::IconType> Billboard::requirementTableConversor =
	{
		{ LogicRequirement::Workers, IconType::SOUL },
		{ LogicRequirement::Energy, IconType::POWER_GENERATOR },
		{ LogicRequirement::Player, IconType::REPAIR },
		{ LogicRequirement::Undefined, IconType::NONE },
	};

	Billboard::Billboard() : IComponent(), _bbSet(nullptr)
	{
	}

	Billboard::~Billboard()
	{
		if (_bbSet)
		{
			delete _bbSet;
			_bbSet = nullptr;
		}
	}

	bool Billboard::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		if (entityInfo->hasAttribute("billboardMaterial"))
		{
			std::string materialName = entityInfo->getStringAttribute("billboardMaterial");
			
			// Por defecto los edificios no tienen icono
			IconType::IconType defaultIcon = IconType::IconType::NONE;
			if (entityInfo->hasAttribute("defaultBillboard")){
				std::string defaultIconStr = entityInfo->getStringAttribute("defaultBillboard");

				if (!defaultIconStr.empty())
					defaultIcon = iconTableConversor.at(defaultIconStr);
			}
						
			_bbSet = new Graphics::BillboardSet(_entity->getComponent<Logic::CGraphics>()->getGraphicsEntity(), "bbSet", materialName);
			
			if (defaultIcon != IconType::NONE){
				_bbSet->createBillboard(Vector3(0.0f, 0.0f, 0.0f));
				IconMessage m(defaultIcon);
				bool result = m.Dispatch(*this);
				assert(result && "Can't set initial icon");
			}

			//_entity->getComponent<Graphics>()
		}

		return true;
	}

	void Billboard::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	bool Billboard::HandleMessage(const IconMessage& msg)
	{
		// TODO TEST solución temporal hasta que se quiten las llamadas al Billboard antes de su inicialización
		if (_bbSet == nullptr)
			return true;

		switch (msg._type)
		{
		case MessageType::ICON_ADD:
		{
			// Avoid adding the same icon twice
			bool isNew = true;
			for (unsigned int i = 0; i < _bbSet->getNumBillboards(); ++i)
			{
				if (_bbSet->getBillboard(i)->getTexcoordIndex() == msg._icon)
				{
					isNew = false;
					break;
				}
			}

			// Only add if it's new
			if (isNew)
			{
				_bbSet->createBillboard(Vector3(0.0f, 0.0f, 0.0f))->setTexcoordIndex(msg._icon);
				adjustBillboards();
			}
			break;
		}
		case MessageType::ICON_CHANGE:
			for (unsigned int i = 0; i < _bbSet->getNumBillboards(); ++i)
			{
				if (_bbSet->getBillboard(i)->getTexcoordIndex() == msg._icon)
					_bbSet->getBillboard(i)->setTexcoordIndex(msg._iconReplacement);
			}
			break;
		case MessageType::ICON_DELETE:
			for (unsigned int i = 0; i < _bbSet->getNumBillboards(); ++i)
			{
				if (_bbSet->getBillboard(i)->getTexcoordIndex() == msg._icon)
					_bbSet->removeBillboard(i);
			}
			adjustBillboards();
			break;
		case MessageType::ICON:
		default:
			_bbSet->getBillboard(0)->setTexcoordIndex(msg._icon);
		}

		return true;
	}

	void Billboard::adjustBillboardsPosition(const std::vector<std::pair<float,float>>& grid) const
	{
		for (unsigned int i = 0; i < grid.size(); ++i)
		{
			_bbSet->getBillboard(i)->setPosition(grid[i].first, grid[i].second, 0.0f);
		}
	}
	
	void Billboard::fillGrid(const unsigned int numBillboards, std::vector<std::pair<float, float>>& grid) const
	{
		// Odd numbers should have one more element in upper row
		const unsigned int BILLBOARDS_PER_ROW = ceil(numBillboards / 2.0f);
		const float HALF_SPACE_BETWEEN = 0.5f;

		const float HEIGHT = 1.f;

		// Size of grid
		const unsigned int Y = (numBillboards == 2) ? 1 : 2; // Use 1 row for 2 billboard, 2 rows for 3 or more

		for (unsigned int y = 0; y < Y; ++y)
		{
			for (unsigned int x = 0; x < BILLBOARDS_PER_ROW; ++x)
			{
				grid.push_back(std::make_pair((x * HALF_SPACE_BETWEEN) - (HALF_SPACE_BETWEEN) + HALF_SPACE_BETWEEN / 2, y * HEIGHT));
			}
		}

		// Remove element for odd size
		if (numBillboards % 3 == 0)
			grid.erase(--grid.end());
	}

	std::vector<std::pair<float, float>> Billboard::createGrid(const unsigned int numBillboards) const
	{
		std::vector<std::pair<float, float>> grid;

		fillGrid(numBillboards, grid);

		return grid;
	}

	void Billboard::adjustBillboards() const
	{
		unsigned int numBillboards = _bbSet->getNumBillboards();

		if (numBillboards == 0)
		{
			return;
		}
		else if (numBillboards == 1)
		{
			_bbSet->getBillboard(0)->setPosition(Vector3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			std::vector<std::pair<float, float>> positions = createGrid(numBillboards);

			adjustBillboardsPosition(positions);
		}
	}

	/*
	IconType::IconType Billboard::getIcon(unsigned int i) const
	{
		return static_cast<IconType::IconType>(_bbSet->getBillboard(i)->getTexcoordIndex());
	}
	*/

	IconType::IconType Billboard::getResourceIcon(ResourceType type)
	{
		return resourceTableConversor.at(type);
	}

	IconType::IconType Billboard::getBuildingIcon(BuildingType type)
	{
		return buildingTableConversor.at(type);
	}

	IconType::IconType Billboard::getLogicRequirementIcon(LogicRequirement type)
	{
		return requirementTableConversor.at(type);
	}

	IconType::IconType Billboard::getStorageIcon(ResourceType type)
	{
		return storageTableConversor.at(type);
	}
}