// @author Asier González

#include "BillboardSet.h"

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Graphics
{
	BillboardSet::BillboardSet(Graphics::CEntity* entity, const std::string& name, const std::string& materialName, const Vector3& position)
	{
			CScene* scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
			Ogre::BillboardSet* bbSet = scene->createBillboardSet(entity, name, position);
			bbSet->setMaterialName(materialName);
			bbSet->setDefaultDimensions(1, 1);

			// Specific parts
			if (materialName == "Billboard/TrueInfernoIcons")
			{
				bbSet->setTextureStacksAndSlices(1, 29);
			}
			else if (materialName == "Billboard/TrueInfernoNewIcons")
			{
				bbSet->setTextureStacksAndSlices(4, 7);
			}

			_bbSet = bbSet;
	}

	BillboardSet::~BillboardSet()
	{
	}

	Ogre::BillboardSet* BillboardSet::getBillboardSet() const
	{
		return _bbSet;
	}

	Ogre::Billboard* BillboardSet::createBillboard(Vector3& vector) const
	{
		return _bbSet->createBillboard(vector);
	}

	Ogre::Billboard* BillboardSet::getBillboard(unsigned int index) const
	{
		return _bbSet->getBillboard(index);
	}

	void BillboardSet::removeBillboard(unsigned int index) const
	{
		_bbSet->removeBillboard(index);
	}

	unsigned int BillboardSet::getNumBillboards() const
	{
		return _bbSet->getNumBillboards();
	}
}