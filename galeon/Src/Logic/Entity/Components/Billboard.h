// @author Asier González

#ifndef __Logic_Billboard_H
#define __Logic_Billboard_H

#include <unordered_map>

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Billboard;
}

namespace Graphics
{
	class BillboardSet;
}

namespace Logic
{
	enum ResourceType;
	enum BuildingType;
	enum LogicRequirement;

	namespace IconType
	{
		enum IconType;
	}
}

namespace Logic
{
	class Billboard : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(Billboard);
	
	public:
		Billboard();
		virtual ~Billboard();

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual void tick(unsigned int msecs);
		bool HandleMessage(const IconMessage& msg);
		//IconType::IconType getIcon(unsigned int) const;
		static IconType::IconType getResourceIcon(ResourceType type);
		static IconType::IconType getBuildingIcon(BuildingType type);
		static IconType::IconType getLogicRequirementIcon(LogicRequirement type);
		static IconType::IconType getStorageIcon(ResourceType type);

	protected:
		static const std::unordered_map<std::string, IconType::IconType> iconTableConversor;
		static const std::unordered_map<ResourceType, IconType::IconType> resourceTableConversor;
		static const std::unordered_map<BuildingType, IconType::IconType> buildingTableConversor;
		static const std::unordered_map<LogicRequirement, IconType::IconType> requirementTableConversor;
		static const std::unordered_map<ResourceType, IconType::IconType> storageTableConversor;

		void adjustBillboards() const;
		void adjustBillboardsPosition(const std::vector<std::pair<float, float>>& grid) const;
		void fillGrid(const unsigned numBillboards, std::vector<std::pair<float, float>>& grid) const;
		std::vector<std::pair<float, float>> createGrid(const unsigned int numBillboards) const;
		
		Graphics::BillboardSet* _bbSet;
	};

	REG_FACTORY(Billboard);
}

#endif //__Logic_Billboard_H