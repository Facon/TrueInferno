// @author Asier González

#ifndef __Graphics_Billboard_H
#define __Graphics_Billboard_H

#include <string>
#include <map>
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
	// This class use Flyweight design pattern because we need to share resources between multiple instances
	// without using more memory
	class BillboardSet
	{
	protected:
		static std::map<std::string, Ogre::BillboardSet*> bbSets;

	public:
		BillboardSet(Graphics::CEntity* entity, const std::string& name, const std::string& materialName);
		virtual ~BillboardSet();

		Ogre::Billboard* createBillboard(Vector3& vector);
		Ogre::Billboard* getBillboard(unsigned int index);
		void removeBillboard(unsigned int index);
		Ogre::BillboardSet* getBillboardSet();
		unsigned int getNumBillboards();

	protected:
		Ogre::BillboardSet* _bbSet;
	};
}

#endif //__Graphics_Billboard_H