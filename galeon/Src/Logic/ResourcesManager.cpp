#include "ResourcesManager.h"

#include "Application/GameState.h"
#include "HFManager.h"
#include "BuildingManager.h"
#include "Logic\Entity\Components\ResourceBuilding.h"
#include "BaseSubsystems\ScriptManager.h"

#include <cassert>

namespace Logic
{
	ResourcesManager ResourcesManager::_instance;

	ResourcesManager* ResourcesManager::getSingletonPtr()
	{
		return &_instance;
	}

	CurrentResourcesMap ResourcesManager::createCurrentResourcesMap()
	{
		CurrentResourcesMap map =
		{
			{ ResourceType::MINERAL, 0 },
			{ ResourceType::GAS, 0 },
			{ ResourceType::COKE, 0 },
			{ ResourceType::CRUDE, 0 },
			{ ResourceType::PURE_EVIL, 0 },
			{ ResourceType::REFINED, 0 },
			{ ResourceType::AETHER, 0 },
		};

		return map;
	}

	MaxResourcesMap ResourcesManager::createMaxResourcesMap()
	{
		MaxResourcesMap map =
		{
			{ ResourceType::MINERAL, 0 },
			{ ResourceType::GAS, 0 },
			{ ResourceType::COKE, 0 },
			{ ResourceType::CRUDE, 0 },
			{ ResourceType::PURE_EVIL, 0 },
			{ ResourceType::REFINED, 0 },
			{ ResourceType::AETHER, 0 },
		};

		return map;
	}

	ResourcesManager::ResourcesManager() : _currentResources(createCurrentResourcesMap()), _maxResources(createMaxResourcesMap())
	{
	}

	ResourcesManager::~ResourcesManager()
	{
	} // ~ResourcesManager

	bool ResourcesManager::Init()
	{
		if (!_instance.open())
		{
			Release();
			return false;
		}

		return true;
	} // Init

	void ResourcesManager::Release()
	{
		_instance.close();
	} // Release

	bool ResourcesManager::open() {
		luaRegister();

		_currentResources[ResourceType::MINERAL] = 0;
		_currentResources[ResourceType::GAS] = 0;
		_currentResources[ResourceType::COKE] = 0;
		_currentResources[ResourceType::CRUDE] = 0;
		_currentResources[ResourceType::PURE_EVIL] = 0;
		_currentResources[ResourceType::REFINED] = 0;
		_currentResources[ResourceType::AETHER] = 0;

		_maxResources[ResourceType::MINERAL] = 0;
		_maxResources[ResourceType::GAS] = 0;
		_maxResources[ResourceType::COKE] = 0;
		_maxResources[ResourceType::CRUDE] = 0;
		_maxResources[ResourceType::PURE_EVIL] = 0;
		_maxResources[ResourceType::REFINED] = 0;
		_maxResources[ResourceType::AETHER] = 0;

		return true;

	} // open

	void ResourcesManager::close() {

	} // close

	void ResourcesManager::luaRegister() {
		// ResourcesManager.
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<ResourcesManager>("ResourcesManager")
				.enum_("ResourceType")
				[
					luabind::value("RT_NONE", ResourceType::NONE),
					luabind::value("RT_MINERAL", ResourceType::MINERAL),
					luabind::value("RT_GAS", ResourceType::GAS),
					luabind::value("RT_COKE", ResourceType::COKE),
					luabind::value("RT_CRUDE", ResourceType::CRUDE),
					luabind::value("RT_PURE", ResourceType::PURE_EVIL),
					luabind::value("RT_REFINED", ResourceType::REFINED),
					luabind::value("RT_AETHER", ResourceType::AETHER)
				]
			];

	} // luaRegister
	
	int ResourcesManager::getDisplayedResources(ResourceType type) const {
		return static_cast<int>(truncf(_currentResources.at(type)));
	}

	void ResourcesManager::changeDisplayedResources(ResourceType type, float num) {
		if (num == 0)
			return;

		// Todos los recursos, salvo el favor de Hades, se actualizan en el mapa gen�rico
		if (type != ResourceType::HADES_FAVOR) {
			_currentResources[type] += num;
		}

		// Pero si es favor de Hades
		else{
			// Se transmite la petici�n de cambio al HFManager
			HFManager* hfManager = HFManager::getSingletonPtr();
			hfManager->changeHadesFavor(num);
		}
	}

	int ResourcesManager::getDisplayedMaxResources(ResourceType type) const {
		return _maxResources.at(type);
	}

	void ResourcesManager::changeDisplayedMaxResources(ResourceType type, int num) {
		// Hades' Favor tiene un m�ximo ilimitado
		// TODO ERROR Mirar porque el favor de hades aparece aqu�
		if (type != ResourceType::HADES_FAVOR) {
			_maxResources[type] += num;
		}
	}

	ResourceType ResourcesManager::parseResourceType(const std::string& name){
		if (name == "MINERAL")
			return MINERAL;
		else if (name == "GAS")
			return GAS;
		else if (name == "COKE")
			return COKE;
		else if (name == "CRUDE")
			return CRUDE;
		else if (name == "PURE_EVIL")
			return PURE_EVIL;
		else if (name == "REFINED")
			return REFINED;
		else if (name == "AETHER")
			return AETHER;
		else if (name == "HADES_FAVOR")
			return HADES_FAVOR;
		else
		{
			assert(false && "ResourceType name unknown");
			return NONE;
		}
	}

	std::unordered_map<CResourceBuilding*, int> ResourcesManager::findResources(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalAvailable){
		// Obtenemos todos los recursos disponibles indexados por componente
		std::unordered_map<CResourceBuilding*, int> availabilityMap;

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

	std::unordered_map<CResourceBuilding*, int> ResourcesManager::findStorage(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalStorage){
		// Obtenemos todos los recursos almacenables indexados por componente
		std::unordered_map<CResourceBuilding*, int> storageMap;

		// Y el total
		totalStorage = 0;

		for (auto itBuilding = resourceBuildings.cbegin(); itBuilding != resourceBuildings.cend(); ++itBuilding) {
			// La capacidad de un edificio es la capacidad m�xima menos lo que tiene almacenado
			int buildingStorage = (*itBuilding)->getMaxResources(type) - (*itBuilding)->getStoredResources(type);

			// Si tiene algo de capacidad almacenable de recursos
			if (buildingStorage > 0){
				// Indexamos el edificio
				storageMap[(*itBuilding)->getEntity()->getComponent<CResourceBuilding>()] = buildingStorage;

				// Agregamos su disponibilidad al total
				totalStorage += buildingStorage;
			}
		}

		return storageMap;
	}

	bool ResourcesManager::decrementResources(ResourceType type, int decrement, bool onlyCheck, bool allowPartial, int& finalDecrement){
		assert(decrement >= 0 && "Decrement must be positive");

		// Controlamos decrementos negativos � 0
		if (decrement <= 0){
			finalDecrement = 0;
			return true;
		}

		// Obtenemos todos los ResourceBuilding entre los edificios
		std::vector<CResourceBuilding*> resourceBuildings = CBuildingManager::getSingletonPtr()->getBuildings<CResourceBuilding>();

		// Obtenemos la informaci�n de disponibilidad de recursos
		int totalAvailable;
		std::unordered_map<CResourceBuilding*, int> availabilityMap = findResources(type, resourceBuildings, totalAvailable);

		// Vemos si el total disponible cubre el decremento solicitado completo
		// Si cubre
		if (totalAvailable >= decrement){
			// Podemos decrementar el total
			finalDecrement = decrement;
		}

		// Si no
		else{
			// Fallamos si no se permiten decrementos parciales
			if (!allowPartial){
				finalDecrement = 0;
				return false;
			}

			// Si se permiten, marcamos que podemos decrementar hasta lo disponible
			finalDecrement = totalAvailable;
		}

		// Si s�lo hab�a que chequear, finalizamos con �xito
		if (onlyCheck)
			return true;

		// Modificamos recursos en cada edificio repartiendo de forma proporcional a lo que tengan
		// TODO Covendr�a ordenar de mayor a menor capacidad para que los flecos del reparto se asignen al que m�s tiene
		int totalProcessed = finalDecrement;
		for (auto it = availabilityMap.cbegin(); it != availabilityMap.cend(); ++it){
			// Calculamos la proporci�n que solicitaremos al edificio
			float proportion = (*it).second * 1.0 / totalAvailable;
			
			// Y la hacemos relativa al decremento final que vamos a realizar
			// Redondeamos hacia arriba para evitar repartir un decremento, p.ej. de 100 entre 3 (-> 33) que resultar�a en un decremento err�neao de 99
			int buildingRequested = (int) ceil(proportion * finalDecrement);

			// Ajustamos a lo que nos quede para no pedir de m�s
			// e.g. 100 entre 3: 1� dar�a 34, 2� dar�a 34, 3� dar�a 32
			buildingRequested = fmin(buildingRequested, totalProcessed);

			(it->first)->changeStoredResources(type, -buildingRequested);

			// Vamos quitando del total restante porque con el redondeo a entero podr�amos acabar antes de llegar a la �ltima entidad
			totalProcessed -= buildingRequested;

			// Paramos si ya se solicit� todo lo que necesit�bamos
			if (totalProcessed <= 0){
				break;
			}
		}

		// El reparto deber�a cuadrar perfectamente
		assert(totalProcessed == 0 && "Error in resource decrement distribution algorithm");

		return true;
	}

	bool ResourcesManager::incrementResources(ResourceType type, int increment, bool onlyCheck, bool allowPartial, int& finalIncrement){
		assert(increment >= 0 && "Decrement must be positive");

		// Controlamos decrementos negativos � 0
		if (increment <= 0){
			finalIncrement = 0;
			return true;
		}

		// Obtenemos todos los ResourceBuilding entre los edificios
		std::vector<CResourceBuilding*> resourceBuildings = CBuildingManager::getSingletonPtr()->getBuildings<CResourceBuilding>();

		// Obtenemos la informaci�n de disponibilidad de recursos
		int totalAvailable;
		std::unordered_map<CResourceBuilding*, int> storageMap = findStorage(type, resourceBuildings, totalAvailable);

		// Vemos si el total disponible cubre el incremento solicitado completo
		// Si cubre
		if (totalAvailable >= increment){
			// Podemos incrementar el total
			finalIncrement = increment;
		}

		// Si no
		else{
			// Fallamos si no se permiten incrementos parciales
			if (!allowPartial){
				finalIncrement = 0;
				return false;
			}

			// Si se permiten, marcamos que podemos incrementar hasta lo disponible
			finalIncrement = totalAvailable;
		}

		// Si s�lo hab�a que chequear, finalizamos con �xito
		if (onlyCheck)
			return true;

		// Modificamos recursos en cada edificio repartiendo de forma proporcional a lo que tengan
		// TODO Covendr�a ordenar de mayor a menor capacidad para que los flecos del reparto se asignen al que m�s capacidad tiene
		int totalProcessed = finalIncrement;
		for (auto it = storageMap.cbegin(); it != storageMap.cend(); ++it){
			// Calculamos la proporci�n que solicitaremos al edificio
			float proportion = (*it).second * 1.0 / totalAvailable;

			// Y la hacemos relativa al incremento final que vamos a realizar
			// Redondeamos hacia arriba para evitar repartir un incremento, p.ej. de 100 entre 3 (-> 33) que resultar�a en un incremento err�neao de 99
			int buildingRequested = (int)ceil(proportion * finalIncrement);

			// Ajustamos a lo que nos quede para no pedir de m�s
			// e.g. 100 entre 3: 1� dar�a 34, 2� dar�a 34, 3� dar�a 32
			buildingRequested = fmin(buildingRequested, totalProcessed);

			(it->first)->changeStoredResources(type, buildingRequested);

			// Vamos quitando del total restante porque con el redondeo a entero podr�amos acabar antes de llegar a la �ltima entidad
			totalProcessed -= buildingRequested;

			// Paramos si ya se solicit� todo lo que necesit�bamos
			if (totalProcessed <= 0){
				break;
			}
		}

		// El reparto deber�a cuadrar perfectamente
		assert(totalProcessed == 0 && "Error in resource increment distribution algorithm");

		return true;
	}
}