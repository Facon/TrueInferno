
#ifndef BUILDINGSELECTION_H_
#define BUILDINGSELECTION_H_

#include "Logic/Entity/Component.h"
#include <string>

namespace Logic {
	class CBuildingSelection : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CBuildingSelection);

	public:
		
		CBuildingSelection() {}
		
		~CBuildingSelection() {}


		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		std::string getPopupLayoutTemplate();
		std::string getSidebarLayoutTemplate();
		std::string getBuildingName();
		std::string getBuildingImage();

	private:
		std::string _popupLayoutTemplate;
		std::string _sidebarLayoutTemplate;
		std::string _buildingName;
		std::string _buildingImage;
	}; 

	REG_FACTORY(CBuildingSelection);

} 

#endif // BUILDINGSELECTION_H_
