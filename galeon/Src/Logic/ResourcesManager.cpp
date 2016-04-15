#include "ResourcesManager.h"
#include <stdexcept>
#include <cassert>

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"
#include "HFManager.h"

namespace Logic
{
	/*const float ResourcesManager::MINERAL_GATHERING_SPEED = 20.f / 60.f;
	const float ResourcesManager::GAS_GATHERING_SPEED = 12.f / 60.f;*/

	/*void ResourcesManager::incMineral(int workers, float time)
	{
		increaseResources(ResourceType::MINERAL, MINERAL_GATHERING_SPEED * workers * time);
	}*/
	
	/*void ResourcesManager::incGas(int workers, float time)
	{
		increaseResources(ResourceType::GAS, GAS_GATHERING_SPEED * workers * time);
	}*/
	
	ResourcesManager ResourcesManager::_instance = ResourcesManager();

	void ResourcesManager::changeResources(ResourceType type, float num)
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
			case ResourceType::HADES_FAVOR:{
				// TODO Atajo temporal para obtener el HFManager
				HFManager* hfManager = HFManager::getSingletonPtr();
				hfManager->setHadesFavor(hfManager->getHadesFavor() + num);
			}
				break;
			default:
				throw std::runtime_error("Invalid Resource.");
		}
	}

	ResourceType ResourcesManager::parseResourceType(const std::string& name){
		if (name == "MINERAL"){
			return MINERAL;
		}
		else if (name == "GAS"){
			return GAS;
		}
		else if (name == "COKE"){
			return COKE;
		}
		else if (name == "CRUDE"){
			return CRUDE;
		}
		else if (name == "PURE"){
			return PURE;
		}
		else if (name == "REFINED"){
			return REFINED;
		}
		else if (name == "AETHER"){
			return AETHER;
		}
		else if (name == "HADES_FAVOR"){
			return HADES_FAVOR;
		}
		else{
			assert(false && "ResourceType name unknown");
			return NONE;
		}
	}

}