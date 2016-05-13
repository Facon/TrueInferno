#ifndef __Logic_Billboard_H
#define __Logic_Billboard_H

#include <unordered_map>

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "Graphics/BillboardSet.h"

namespace Ogre
{
	class Billboard;
}

namespace Logic
{
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

	protected:
		static const std::unordered_map<std::string, IconType::IconType> iconTableConversor;

		Graphics::BillboardSet* _bbSet;
		Ogre::Billboard* _billboard;
	};

	REG_FACTORY(Billboard);
}

#endif //__Logic_Billboard_H