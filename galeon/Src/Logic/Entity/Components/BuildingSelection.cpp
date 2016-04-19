
#include "BuildingSelection.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include <iostream>

namespace Logic {
	RTTI_ROOT_IMPL(CBuildingSelection);
	IMP_FACTORY(CBuildingSelection);

	bool CBuildingSelection::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if (entityInfo->hasAttribute("sidebarlayout"))
			_sidebarLayoutTemplate = entityInfo->getStringAttribute("sidebarlayout");
		if (entityInfo->hasAttribute("popuplayout"))
			_popupLayoutTemplate = entityInfo->getStringAttribute("popuplayout");
		if (entityInfo->hasAttribute("type"))
			_buildingName = entityInfo->getStringAttribute("type");
		if (entityInfo->hasAttribute("image"))
			_buildingImage = entityInfo->getStringAttribute("image");
		return true;
	} // spawn

	std::string CBuildingSelection::getSidebarLayoutTemplate(){
		return _sidebarLayoutTemplate;
	}

	std::string CBuildingSelection::getPopupLayoutTemplate(){
		return _popupLayoutTemplate;
	}

	std::string CBuildingSelection::getBuildingName(){
		return _buildingName;
	}

	std::string CBuildingSelection::getBuildingImage(){
		return _buildingImage;
	}

	void CBuildingSelection::tick(unsigned int msecs){

	} // tick

} // namespace Logic
