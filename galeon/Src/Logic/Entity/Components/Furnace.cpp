#include "Furnace.h"

namespace Logic
{
	RTTI_ROOT_IMPL(CFurnace);
	IMP_FACTORY(CFurnace);

	bool CFurnace::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if (!IComponent::spawn(entity, map, entityInfo))
			return false;

		return true;
	} // spawn

	void CFurnace::tick(unsigned int msecs){
		IComponent::tick(msecs);
	} // tick

	bool CFurnace::HandleMessage(const ResourceMessage& msg){
		if (msg._type != MessageType::RESOURCES_CHANGE)
			return false;

		if (msg._resourceType != ResourceType::COKE){
			_coke += msg._number;
			std::cout << "Coke=" << _coke << std::endl;
			// TODO Notify Logic::ResourcesManager
		}
		else if (msg._resourceType != ResourceType::CRUDE){
			_crude += msg._number;
			std::cout << "Crude=" << _crude << std::endl;
			// TODO Notify Logic::ResourcesManager
		}
		else{
			assert(false && "Resource not handled");
			return false;
		}

		return true;
	}

}