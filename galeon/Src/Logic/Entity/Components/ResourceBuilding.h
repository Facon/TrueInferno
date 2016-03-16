#ifndef RESOURCE_BUILDING_
#define RESOURCE_BUILDING_

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include <unordered_map>

namespace Logic {
	class CResourceBuilding : public IComponent {
		RTTI_DECL;
		DEC_FACTORY(CResourceBuilding);

	public:
		CResourceBuilding() {}
		
		virtual ~CResourceBuilding() { 
			_storedResources.clear(); 
			_providedResources.clear();
		}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool HandleMessage(const ResourceMessage& msg);

		// TODO Interfaz o mensajes para tratar disponibilidad de recursos

	private:
		/** Cantidad de recursos almacenados indexados por tipo */
		std::unordered_map<ResourceType, int> _storedResources;

		/** Recursos que se proveen */
		std::unordered_set<ResourceType> _providedResources;
	};
	
	REG_FACTORY(CResourceBuilding);

} // namespace Logic

#endif // RESOURCE_BUILDING_