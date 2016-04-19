#include "ResourcesManager.h"

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"
#include "HFManager.h"
#include <algorithm>
#include "BuildingManager.h"
#include "Logic\Entity\Components\ResourceBuilding.h"

#include <stdexcept>
#include <cassert>
#include <map>

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

	std::map<CResourceBuilding*, int> ResourcesManager::findResources(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalAvailable){
		// Obtenemos todos los recursos disponibles indexados por entidad
		std::map<CResourceBuilding*, int> availabilityMap;

		// Y el total
		totalAvailable = 0;

		for (auto itBuilding = resourceBuildings.cbegin(); itBuilding != resourceBuildings.cend(); ++itBuilding) {
			int buildingAvailable = (*itBuilding)->getAvailableResources(type);

			// Si tiene algo de recurso disponible
			if (buildingAvailable > 0){
				// Indexamos el edificio
				availabilityMap[(*itBuilding)->getEntity()->getComponent<CResourceBuilding>()] = buildingAvailable;

				// Agregamos su disponibilidad al total
				totalAvailable += buildingAvailable;
			}
		}

		return availabilityMap;
	}

	bool ResourcesManager::payCost(ResourceType type, int cost, bool onlyCheck, bool allowPartial, int& finalCost){
		// Controlamos costes negativos ó 0
		if (cost <= 0){
			assert(cost >= 0 && "Cost must be positive");
			finalCost = 0;
			return true;
		}

		// Obtenemos todos los ResourceBuilding entre los edificios
		std::vector<CResourceBuilding*> resourceBuildings = CBuildingManager::getSingletonPtr()->getBuildings<CResourceBuilding>();

		// Obtenemos la información de disponibilidad de recursos
		int totalAvailable;
		std::map<CResourceBuilding*, int> availabilityMap = findResources(type, resourceBuildings, totalAvailable);

		// Vemos si el total disponible cubre el coste completo
		// Si cubre
		if (totalAvailable >= cost){
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
			finalCost = totalAvailable;
		}

		// Si sólo había que chequear, finalizamos con éxito
		if (onlyCheck)
			return true;

		// Modificamos recursos en cada edificio repartiendo de forma proporcional a lo que tengan
		// TODO Covendría ordenar de mayor a menor capacidad para que los flecos del reparto se asignen al que más tiene
		int totalRequested = finalCost;
		for (auto it = availabilityMap.cbegin(); it != availabilityMap.cend(); ++it){
			// Calculamos la proporción que solicitaremos al edificio
			float proportion = (*it).second * 1.0 / totalAvailable;
			
			// Y la hacemos relativa al coste final que vamos a pagar
			// Redondeamos hacia arriba para evitar repartir un pago, p.ej. de 100 entre 3 (-> 33) que resultaría en un pago de 99
			int buildingRequested = (int) ceil(proportion * finalCost);

			// Ajustamos a lo que nos quede para no pedir de más
			// e.g. 100 entre 3: 1º daría 34, 2º daría 34, 3º daría 32
			buildingRequested = fmin(buildingRequested, totalRequested);

			(it->first)->changeStoredResources(type, -buildingRequested);

			// Vamos sumando porque con el redondeo a entero podríamos acabar antes
			totalRequested -= buildingRequested;

			// Paramos si ya se solicitó todo lo que necesitábamos
			if (totalRequested <= 0){
				break;
			}
		}

		// El reparto debería cuadrar perfectamente
		assert(totalRequested == 0 && "Error in cost payment algorithm");

		return true;
	}

}