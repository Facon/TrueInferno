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
	
	void ResourcesManager::increaseResources(ResourceType type, float num)
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
				HFManager *hfManager =
					((Application::CGameState*) Application::CGaleonApplication::getSingletonPtr()->getState())->getHFManager();
				hfManager->setHadesFavor(hfManager->getHadesFavor() + num);
			}
				break;
			default:
				throw std::runtime_error("Invalid Resource.");
		}
	}

	bool ResourcesManager::decreaseResources(ResourceType type, float num)
	{
		switch (type)
		{
		case ResourceType::MINERAL:
			if (_mineral < num)
				return false;
			_mineral -= num;
			break;
		case ResourceType::GAS:
			if (_gas < num)
				return false;
			_gas -= num;
			break;
		case ResourceType::COKE:
			if (_coke < num)
				return false;
			_coke -= num;
			break;
		case ResourceType::CRUDE:
			if (_crude < num)
				return false;
			_crude -= num;
			break;
		case ResourceType::PURE:
			if (_pure < num)
				return false;
			_pure -= num;
			break;
		case ResourceType::REFINED:
			if (_refined < num)
				return false;
			_refined -= num;
			break;
		case ResourceType::AETHER:
			if (_aether < num)
				return false;
			_aether -= num;
			break;
			/*
			case ResourceType::HADES_FAVOR:
			_hadesFavor += num;
			break;
			*/
		default:
			throw std::runtime_error("Invalid Resource.");
		}
		return true;
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