#ifndef FURNACE_H_
#define FURNACE_H_

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

namespace Logic {
	class CFurnace : public IComponent {
		RTTI_DECL;
		DEC_FACTORY(CFurnace);

	public:
		CFurnace() : _coke(0), _crude(0) {}
		virtual ~CFurnace() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool HandleMessage(const ResourceMessage& msg);

	private:
		/** Cantidad de coke acumulado */
		int _coke;

		/** Cantidad de crude acumulado */
		int _crude;
	};
	
	REG_FACTORY(CFurnace);

} // namespace Logic

#endif // FURNACE_H_