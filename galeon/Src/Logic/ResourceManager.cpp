#include "ResourceManager.h"
#include <stdexcept>

namespace Logic
{
	const float ResourceManager::MINERAL_GATHERING_SPEED = 20.f / 60.f;
	const float ResourceManager::GAS_GATHERING_SPEED = 12.f / 60.f;

	void ResourceManager::incMineral(int workers, float time)
	{
		increaseResources(ResourceType::MINERAL, MINERAL_GATHERING_SPEED * workers * time);
	}
	
	void ResourceManager::incGas(int workers, float time)
	{
		increaseResources(ResourceType::GAS, GAS_GATHERING_SPEED * workers * time);
	}
	
	void ResourceManager::increaseResources(ResourceType type, float num)
	{
		switch(type)
		{
			case ResourceType::MINERAL:
				_mineral += num;
				break;
			case ResourceType::GAS:
				_gas += num;
				break;
			case ResourceType::COKE:
				_coke += num; 
				break;
			case ResourceType::CRUDE:
				_crude += num;
				break;
			case ResourceType::PURE:
				_pure += num;
				break;
			case ResourceType::REFINED:
				_refined += num;
				break;
			case ResourceType::AETHER:
				_aether += num;
				break;
			case ResourceType::HADES_FAVOR:
				_hadesFavor += num;
				break;
			default:
				throw std::runtime_error("Invalid Resource.");
		}
	}
}