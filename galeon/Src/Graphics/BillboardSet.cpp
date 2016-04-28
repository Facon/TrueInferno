#include "BillboardSet.h"

#include <string>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Graphics
{
	BillboardSet::BillboardSet(Graphics::CEntity* entity, const std::string& name, const std::string& materialName) : _name(name)
	{
		static unsigned int i = 0;
		CScene* scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		_bbSet = scene->createBillboardSet(entity, name + "_" + std::to_string(i));
		_bbSet->setMaterialName(materialName);
		_bbSet->setDefaultDimensions(1, 1);
		++i;
	}

	BillboardSet::~BillboardSet()
	{
	}

	Ogre::Billboard* BillboardSet::createBillboard(Vector3& vector)
	{
		return _bbSet->createBillboard(vector);
	}
}