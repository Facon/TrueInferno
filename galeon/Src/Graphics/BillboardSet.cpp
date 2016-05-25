// @author Asier González

#include "BillboardSet.h"

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

namespace Graphics
{
	//std::map<std::string, Ogre::BillboardSet*> BillboardSet::bbSets;

	BillboardSet::BillboardSet(Graphics::CEntity* entity, const std::string& name, const std::string& materialName)
	{
		//std::map<std::string, Ogre::BillboardSet*>::const_iterator bbSet = bbSets.find(name);

		//if (bbSet == bbSets.cend())
		//{
			CScene* scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
			Ogre::BillboardSet* bbSet = scene->createBillboardSet(entity, name);
			bbSet->setMaterialName(materialName);
			bbSet->setDefaultDimensions(0.5, 0.5);

			// Specific parts
			if (materialName == "Billboard/TrueInfernoIcons")
			{
				bbSet->setTextureStacksAndSlices(1, 28);
			}

			_bbSet = bbSet;
		//	bbSets.insert(std::pair<std::string, Ogre::BillboardSet*>(name, bbSet));
		//}
		//else
		//{
			//_bbSet = bbSet->second;
		//}
	}

	BillboardSet::~BillboardSet()
	{
	}

	Ogre::BillboardSet* BillboardSet::getBillboardSet()
	{
		return _bbSet;
	}

	Ogre::Billboard* BillboardSet::createBillboard(Vector3& vector)
	{
		return _bbSet->createBillboard(vector);
	}

	Ogre::Billboard* BillboardSet::getBillboard(unsigned int index)
	{
		return _bbSet->getBillboard(index);
	}

	void BillboardSet::removeBillboard(unsigned int index)
	{
		_bbSet->removeBillboard(index);
	}

	unsigned int BillboardSet::getNumBillboards()
	{
		return _bbSet->getNumBillboards();
	}
}