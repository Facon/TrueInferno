#ifndef __Graphics_Billboard_H
#define __Graphics_Billboard_H

#include <string>
#include "BaseSubsystems\Math.h"

namespace Ogre
{
	class BillboardSet;
	class Billboard;
	class SceneNode;
}

namespace Graphics
{
	class CEntity;
}

namespace Graphics
{
	class BillboardSet
	{
	public:
		BillboardSet()
		{};
		BillboardSet(Graphics::CEntity* entity, const std::string& name, const std::string& materialName);
		virtual ~BillboardSet();

		Ogre::Billboard* createBillboard(Vector3& vector);

	protected:
		std::string _name;
		Ogre::BillboardSet* _bbSet;
	};
}

#endif //__Graphics_Billboard_H