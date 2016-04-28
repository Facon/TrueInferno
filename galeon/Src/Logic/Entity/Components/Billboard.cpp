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

	Billboard::Billboard() : IComponent()
	{
	}

	bool Billboard::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		if (entityInfo->hasAttribute("billboardMaterial"))
		{
			std::string materialName = entityInfo->getStringAttribute("billboardMaterial");
			_bbSet = Graphics::BillboardSet(_entity->getComponent<Logic::CGraphics>()->getGraphicsEntity(), "bbSet", materialName);
			_billboard = _bbSet.createBillboard(Vector3(0.0f, 0.0f, 0.0f));
			//_entity->getComponent<Graphics>()
		}

		return true;
	}

	void Billboard::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//std::cout << "X: " << _billboard->mParentSet->getParentSceneNode()->getParent()->getPosition().x << " | Y: " << _billboard->mParentSet->getParentSceneNode()->getParent()->getPosition().y << "\n";
	}
}