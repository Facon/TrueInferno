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
				assert(m.Dispatch(*this) && "Can't set initial icon");
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

	void Billboard::adjustBillboards()
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
		else if (numBillboards == 2)
		{
			const double HALF_SPACE_BETWEEN = 0.25;
			_bbSet->getBillboard(0)->setPosition(-HALF_SPACE_BETWEEN, 0, 0);
			_bbSet->getBillboard(1)->setPosition(HALF_SPACE_BETWEEN, 0, 0);
		}
		else
		{
			unsigned int index = 0;
			// Odd numbers should have one more element in upper row
			const unsigned int BILLBOARDS_PER_ROW = (numBillboards / 2) + ((numBillboards % 2) ? 0 : 1);
			const unsigned int ARBITRARY_SPACE = 0.5;
			const double HALF_SPACE_BETWEEN = ARBITRARY_SPACE / BILLBOARDS_PER_ROW;

			for (; index < BILLBOARDS_PER_ROW; ++index)
			{
				//_bbSet->getBillboard(index)->setPosition(BILLBOARDS_PER_ROW * index - HALF_SPACE_BETWEEN, 50.0f, 0.0f);
				_bbSet->getBillboard(index)->setPosition((index * HALF_SPACE_BETWEEN) - BILLBOARDS_PER_ROW * (HALF_SPACE_BETWEEN / 2), 50.0f, 0.0f);
			}

			for (; index < numBillboards; ++index)
			{
				_bbSet->getBillboard(index)->setPosition(((index - BILLBOARDS_PER_ROW) * HALF_SPACE_BETWEEN) - (BILLBOARDS_PER_ROW - index) * (HALF_SPACE_BETWEEN / 2), 0.0f, 0.0f);
			}
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
}