#ifndef RESOURCE_BUILDING_
#define RESOURCE_BUILDING_

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

namespace Logic {
	class CResourceBuilding : public IComponent {
		RTTI_DECL;
		DEC_FACTORY(CResourceBuilding);

	public:
		CResourceBuilding() : _resourceType(ResourceType::NONE), _resourceQuantity(0) {}
		virtual ~CResourceBuilding() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool HandleMessage(const ResourceMessage& msg);

	private:
		/** Tipo del recurso almacenado */
		ResourceType _resourceType;

		/** Cantidad del recurso almacenado */
		int _resourceQuantity;
	};
	
	REG_FACTORY(CResourceBuilding);

} // namespace Logic

#endif // RESOURCE_BUILDING_