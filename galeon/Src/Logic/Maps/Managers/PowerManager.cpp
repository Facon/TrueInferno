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
#include "Logic/BuildingManager.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/PowerGenerator.h"
#include "Logic/Entity/Components/PowerConsumer.h"
#include "Logic/Entity/Entity.h"
#include <cassert>
#include <limits>

namespace Logic {

	CPowerManager* CPowerManager::_instance = 0;

	//--------------------------------------------------------

	CPowerManager::CPowerManager()
	{
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

} // namespace Logic
