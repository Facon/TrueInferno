#include "ResourceManager.h"

namespace Logic
{
	void ResourceManager::incRes1(int workers, float time)
	{
		increaseResources(ResourceType::Mineral, res1GatheringSpeed * workers * time);
	}
	
	void ResourceManager::incRes2(int workers, float time)
	{
		increaseResources(ResourceType::GAS, res2GatheringSpeed * workers * time)
	}
	
	void ResourceManager::increaseResources(ResourceType type, float num)
	{
		switch(type)
		{
			case ResourceType::MINERAL:
				res1 += num;
				break;
			case ResourceType.GAS:
				res2 += num;
				break;
			case ResourceType.COKE: 
				coke += num; 
				break;
			case ResourceType.CRUDE:
				crude += num;
				break;
			case ResourceType.PURE:
				pure += num;
				break;
			case ResourceType.REFINED:
				refined += num;
				break;
		}
	}
}