#include "ResourceBuilding.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CResourceBuilding);
	IMP_FACTORY(CResourceBuilding);

	bool CResourceBuilding::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		assert(entityInfo->hasAttribute("extractedResource"));
		_resourceType = Logic::ResourcesManager::parseResourceType(entityInfo->getStringAttribute("extractedResource"));

		return true;
	} // spawn

	void CResourceBuilding::tick(unsigned int msecs){
		IComponent::tick(msecs);
	} // tick

	bool CResourceBuilding::HandleMessage(const ResourceMessage& msg){
		if (msg._type != MessageType::RESOURCES_CHANGE)
			return false;

		if (msg._resourceType == _resourceType){
			_resourceQuantity += msg._number;
			std::cout << "Resource(" << _resourceType << ")=" << _resourceQuantity << std::endl;
			// TODO Notify Logic::ResourcesManager
		}
		else{
			assert(false && "Resource not handled");
			return false;
		}

		return true;
	}

}