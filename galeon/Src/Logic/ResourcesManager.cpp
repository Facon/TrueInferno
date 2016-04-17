#include "ResourcesManager.h"
#include <stdexcept>
#include <cassert>

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"
#include "HFManager.h"
#include <algorithm>

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

	int ResourcesManager::getResource(ResourceType type) const {
		return (int) truncf(_resources.at(type));
	}

	void ResourcesManager::changeResources(ResourceType type, float num) {
		// Todos los recursos, salvo el favor de Hades, se obtienen del mapa genérico
		if (type != ResourceType::HADES_FAVOR) {
			_resources[type] += num;
		}

		else{
			// Se transmite la petición de cambio al HFManager
			HFManager* hfManager = HFManager::getSingletonPtr();
			hfManager->setHadesFavor(hfManager->getHadesFavor() + num);
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

	bool ResourcesManager::canAffordCost(ResourceType type, int cost, bool allowPartial, int& finalCost){
		assert((cost >= 0) && "Cost must be positive");

		// Inicializamos
		finalCost = 0;

		// Control de costes negativos ó 0
		if (cost <= 0){
			finalCost = 0;
			return true;
		}

		// TODO Mecanismo antiguo: Restamos directamente aquí en vez de quitar de los edificios de recursos

		// Determinamos cuánto hay disponible: available
		int available = (int)truncf(_resources[type]);

		// Vemos si cubre el coste para saber cuánto se podría pagar: paid
		// Si cubre
		if (available >= cost){
			// Podemos pagar el coste total
			finalCost = cost;
		}

		// Si no
		else{
			// Fallamos si no se permiten pagos parciales
			if (!allowPartial){
				finalCost = 0;
				return false;
			}

			// Si se permiten, marcamos que podemos pagar hasta lo disponible
			finalCost = available;
		}

		return true;
	}

	bool ResourcesManager::payCost(ResourceType type, int cost, bool allowPartial, int& finalCost){
		// Si no podemos permitirnos pagar el coste no hay nada que hacer
		if (!canAffordCost(type, cost, allowPartial, finalCost))
			return false;

		// Modificamos recursos
		changeResources(type, -1.0 * finalCost);

		return true;
	}

}