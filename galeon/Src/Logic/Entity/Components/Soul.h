#ifndef SPIRIT_H_
#define SPIRIT_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

namespace Logic
{
	class Soul : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(Soul);

	public:
		Soul() : IComponent()
		{}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
	
	protected:
		float speed = 0.5f;

	};

	REG_FACTORY(Soul);
}

#endif // SPIRIT_H_