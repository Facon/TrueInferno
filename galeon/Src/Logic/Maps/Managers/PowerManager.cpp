//---------------------------------------------------------------------------
// PowerManager.cpp
//---------------------------------------------------------------------------

/**
@file PowerManager.cpp

Contiene la implementación del gestor de energía.

@see Logic::CPowerManager

@author Álvaro Valera
@date Marzo, 2016
*/

#include "PowerManager.h"
#include "Logic/ResourcesManager.h"
#include "Logic/BuildingManager.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/PowerGenerator.h"
#include "Logic/Entity/Components/PowerConsumer.h"
#include "Logic/Entity/Entity.h"
#include <cassert>
#include <limits>

#include <set>

namespace Logic {

	CPowerManager* CPowerManager::_instance = 0;

	//--------------------------------------------------------

	CPowerManager::CPowerManager() : _refillPeriod(3000), _timeSinceLastRefill(0) {
		_instance = this;

	} // CPowerManager

	//--------------------------------------------------------

	CPowerManager::~CPowerManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CPowerManager

	//--------------------------------------------------------

	bool CPowerManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CPowerManager no permitida!");

		new CPowerManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		_instance->_timeSinceLastRefill = 0;

		return true;

	} // Init

	//--------------------------------------------------------

	void CPowerManager::Release()
	{
		assert(_instance && "Logic::CPowerManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CPowerManager::open()
	{
		_timeSinceLastRefill = 0;
		return true;

	} // open

	//--------------------------------------------------------

	void CPowerManager::close()
	{
	} // close

	TEntityID CPowerManager::findPowerGenerator(CEntity* powerConsumer)
	{
		// Buscaremos el edificio con menor proporción de trabajadores asignados sobre el máximo total
		/*TEntityID target = EntityID::UNASSIGNED;
		float minRatio = FLT_MAX;

		// Obtenemos los edificios
		std::map<BuildingType, std::set<CPlaceable*>*> _buildings = CBuildingManager::getSingletonPtr()->getBuildings();

		// Para cada tipo de edificio registrado
		for (auto it = _buildings.cbegin(); it != _buildings.cend(); ++it){
			// Si nos especificaron coger de un tipo concreto, ignoramos todos los demás tipos
			if (buildingType != BuildingType::Unassigned && buildingType != it->first)
				continue;

			// Para cada uno de sus edificios
			if (it->second != nullptr)
				for (auto it2 = it->second->cbegin(); it2 != it->second->cend(); ++it2){
					CWorkBuilding* building = (*it2)->getEntity()->getComponent<CWorkBuilding>();
					// Ignoramos los edificios que no sean de trabajadores
					if (building == nullptr)
						continue;

					// Ignoramos los edificios ya llenos
					if (building->getAssignedWorkers() >= building->getMaxWorkers())
						continue;

					// Calculamos la proporción de ocupación
					float occupationRatio = ((float)building->getAssignedWorkers()) / building->getMaxWorkers();

					// Y actualizamos, si procede, el mejor ratio encontrado hasta el momento
					if (occupationRatio < minRatio){
						minRatio = occupationRatio;
						target = building->getEntity()->getEntityID();
					}
				}
		}

		return target;*/

		// TODO Devolvemos uno al azar provisionalmente
		CPlaceable* target = CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::PowerGenerator);
		if (target != nullptr)
			return CBuildingManager::getSingletonPtr()->findBuilding(BuildingType::PowerGenerator)->getEntity()->getEntityID();
		else
			return EntityID::UNASSIGNED;
	}


	void CPowerManager::tick(unsigned int msecs){
		// Si pasó suficiente tiempo desde el último reabastecimiento
		_timeSinceLastRefill += msecs;
		if (_timeSinceLastRefill >= _refillPeriod){
			// Reabastecemos
			refillPowerGenerators();

			// Damos tiempo para el siguiente reabastecimiento
			_timeSinceLastRefill -= _refillPeriod;
		}
	}

	void CPowerManager::refillPowerGenerators(){
		// TODO Temporalmente se rastrean todos los edificios hasta encontrar los PowerGenerators. Convendría mantenerlos bien registrados en una única lista

		// Buscamos el edificio con mayor necesidad de combustible según su consumo y reserva actual
		CEntity* target = nullptr;
		int maxNeeded = INT_MIN;

		std::vector<CPowerGenerator*> _generators = CBuildingManager::getSingletonPtr()->getBuildings<CPowerGenerator>();

		// Si no hay generadores no se hace nada
		if (_generators.empty())
			return;

		// Para cada PowerGenerator
		for (auto it = _generators.cbegin(); it != _generators.cend(); ++it){
			CPowerGenerator* generator = (*it);

			// TODO Descartamos los que no están activos

			// Calculamos su necesidad de combustible
			int needed = generator->getConsumptionUnits() - generator->getCurrentReserves();

			// Y actualizamos, si procede, el mejor ratio encontrado hasta el momento
			if (needed > maxNeeded){
				maxNeeded = needed;
				target = generator->getEntity();
			}
		}

		// Si encontramos algún edificio a abastecer
		if (target != nullptr){
			// Pedimos a la logística del edificio que busque los recursos que necesita o todos los que haya disponibles si no necesita nada
			LogisticsMessage m;
			m.assembleDemandResources(ResourceType::COKE, maxNeeded > 0 ? maxNeeded : INT_MAX);

			// TODO ¿Reintentamos o elegimos otro objetivo?
			m.Dispatch(*target);
		}

	}

} // namespace Logic
