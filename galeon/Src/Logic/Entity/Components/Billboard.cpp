#include "Billboard.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"

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
		{ "BURNING", IconType::BURNING }, // WTF?
		{ "SHOVEL", IconType::SHOVEL },
		{ "CLOSE", IconType::CLOSE },
		{ "ROCKS", IconType::ROCKS },
		{ "STAR", IconType::STAR }, // WTF?
		{ "NOT_OK", IconType::NOT_OK },
		{ "FLAMES", IconType::FLAMES },
		{ "SOUL_PATH", IconType::SOUL_PATH },
		{ "COMPASS_ROSE", IconType::COMPASS_ROSE },
		{ "FURNACE", IconType::FURNACE },
		{ "GAS_PLANT", IconType::GAS_PLANT }, // WTF?
		{ "HELLQUARTERS", IconType::HELLQUARTERS }, // WTF?
		{ "MINE", IconType::MINE }, // WTF?
		{ "CROSS_SWORD", IconType::CROSS_SWORD }, // WTF?
		{ "SOUL", IconType::SOUL }, // WTF?
		{ "WIFI", IconType::WIFI }, // WTF?
		{ "TOXIC2", IconType::TOXIC2 }, // WTF?
		{ "EVILATOR", IconType::TOXIC3 }, // WTF?
		{ "REFINERY", IconType::REFINERY },
		{ "REPAIR", IconType::REPAIR },
		{ "RESEARCH_LABS", IconType::RESEARCH_LABS },
		{ "POWER_GENERATOR", IconType::POWER_GENERATOR },
		{ "CLOCK", IconType::CLOCK },
		{ "JUDGEMENT", IconType::JUDGEMENT },
		{ "COGS", IconType::COGS }, // WTF?
		{ "WAREHOUSE", IconType::WAREHOUSE },
	};
	
	Billboard::Billboard() : IComponent()
	{
	}

	Billboard::~Billboard()
	{
		if (_bbSet != nullptr)
		{
			delete _bbSet;
			_bbSet = nullptr;
		}

		if (_billboard != nullptr)
		{
			delete _billboard;
			_billboard = nullptr;
		}
	}

	bool Billboard::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		if (entityInfo->hasAttribute("billboardMaterial"))
		{
			std::string materialName = entityInfo->getStringAttribute("billboardMaterial");
			std::string defaultIcon = entityInfo->getStringAttribute("defaultBillboard");
			_bbSet = new Graphics::BillboardSet(_entity->getComponent<Logic::CGraphics>()->getGraphicsEntity(), "bbSet", materialName);
			_billboard = _bbSet->createBillboard(Vector3(0.0f, 0.0f, 0.0f));

			IconMessage m(iconTableConversor.at(defaultIcon));
			m.Dispatch(*this);
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
		_billboard->setTexcoordIndex(msg._icon);

		return true;
	}
}