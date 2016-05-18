//---------------------------------------------------------------------------
// WorkManager.cpp
//---------------------------------------------------------------------------

/**
@file WorkManager.cpp

Contiene la implementación del gestor de trabajo.

@see Logic::CWorkManager

@author Álvaro Valera
@date Marzo, 2016
*/

#include "WorkManager.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/SoulBurner.h"
#include "Logic/Entity/Entity.h"

#include "AI/WorkSoulTask.h"

#include <cassert>
#include <limits>
#include <algorithm>

namespace Logic {

	CWorkManager* CWorkManager::_instance = 0;

	//--------------------------------------------------------

	CWorkManager::CWorkManager()
	{
		_instance = this;

		for (unsigned int i = 0; i < NUM_BUILDING_GROUPS; ++i)
			_groupsPriority[i] = static_cast<BuildingGroup>(i);

	} // CWorkManager

	//--------------------------------------------------------

	CWorkManager::~CWorkManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CWorkManager

	//--------------------------------------------------------

	bool CWorkManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CWorkManager no permitida!");

		new CWorkManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CWorkManager::Release()
	{
		assert(_instance && "Logic::CWorkManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CWorkManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CWorkManager::close()
	{
	} // close

	//--------------------------------------------------------

	TEntityID CWorkManager::findBuildingToWork(BuildingType buildingType)
	{
		// Buscaremos el edificio con menor proporción de trabajadores asignados sobre el máximo total
		TEntityID target = EntityID::UNASSIGNED;
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

		return target;

	} // findBuildingToWork

	//--------------------------------------------------------

	TEntityID CWorkManager::findFurnace()
	{
		// Buscaremos el horno con mayor capacidad actual para quemar almas
		TEntityID target = EntityID::UNASSIGNED;
		int maxSoulsToBurn = INT_MIN;

		// Obtenemos los hornos (si hay alguno)
		std::set<CPlaceable*>* _furnaces = CBuildingManager::getSingletonPtr()->getBuildings()[BuildingType::Furnace];
		
		if(_furnaces == nullptr)
			return EntityID::UNASSIGNED;

		for (auto it = _furnaces->cbegin(); it != _furnaces->cend(); ++it){
			CEntity* entity = (*it)->getEntity();

			CWorkBuilding* furnace = entity->getComponent<CWorkBuilding>();
			// Los hornos deben ser edificios de trabajadores
			if (furnace == nullptr){
				assert(false && "Furnace must have a WorkBuilding component");
				continue;
			}

			// Ignoramos los edificios no activos
			if (!furnace->isActive())
				continue;

			// Los hornos deben ser quemadores de almas
			CSoulBurner* soulBurner = entity->getComponent<CSoulBurner>();
			if (soulBurner == nullptr){
				assert(false && "Furnace must have a SoulBurner component");
				continue;
			}

			// Obtenemos la capacidad restante del horno
			int remainingSoulsToBurn = soulBurner->getMaxSoulsToBurn() - soulBurner->getCurrentSoulsToBurn();

			// Actualizaremos si...
			bool update = false;

			// ...se mejora la mayor capacidad encontrada hasta el momento
			if (remainingSoulsToBurn > maxSoulsToBurn)
				update = true;

			// TODO ...o si iguala y está más cerca
			/*else if (remainingSoulsToBurn == maxSoulsToBurn){
				//update = 
			}*/

			if (update){
				maxSoulsToBurn = remainingSoulsToBurn;
				target = entity->getEntityID();
			}
		}

		return target;

	} // findFurnace

	//--------------------------------------------------------

	unsigned int CWorkManager::setGroupsPriority(std::pair<BuildingGroup, unsigned int> newGroupsPriority[NUM_BUILDING_GROUPS])
	{
		BuildingGroup groupsOrder[NUM_BUILDING_GROUPS];
		for (unsigned int i = 0; i < NUM_BUILDING_GROUPS; ++i)
			groupsOrder[i] = BuildingGroup::Null;

		// Recorremos y ordenamos la nuevas prioridades recibidas para cada grupo
		for (unsigned int i = 0; i < NUM_BUILDING_GROUPS; ++i)
		{
			auto groupPriority = newGroupsPriority[i];
			unsigned int priority = groupPriority.second;
			
			if (1 <= priority && priority <= NUM_BUILDING_GROUPS)
				groupsOrder[priority - 1] = groupPriority.first;
		}

		// Comprobamos que cada grupo tiene una prioridad entre 1 y
		// NUM_BUILDING_GROUPS, y que ésta es única
		for (unsigned int i = 0; i < NUM_BUILDING_GROUPS; ++i)
			if (groupsOrder[i] == BuildingGroup::Null)
				return i + 1;

		// Todo correcto, por lo que actualizamos la lista de prioridades
		// y reordenamos a los trabajadores
		for (unsigned int i = 0; i < NUM_BUILDING_GROUPS; ++i)
			_groupsPriority[i] = groupsOrder[i];

		reassignWorkers();

		return 0;

	} // setGroupsPriority

	//--------------------------------------------------------

	std::vector<CWorkBuilding*> CWorkManager::getBuildingsFromGroup(int groupPriority)
	{
		std::vector<CWorkBuilding*> buildingsFromGroup;

		if (groupPriority >= NUM_BUILDING_GROUPS)
			return buildingsFromGroup;

		// Grupos de edificios por tipo
		std::map<BuildingGroup, std::set<BuildingType>*> buildingGroups = CBuildingManager::getSingletonPtr()->getBuildingGroups();

		// Grupo i = groupPriority
		BuildingGroup buildingGroup = _groupsPriority[groupPriority];
		assert(buildingGroup != BuildingGroup::Null && "Null building group found while reassigning workers");

		std::set<BuildingType>* buildingTypes = buildingGroups[buildingGroup];
		assert(buildingTypes != nullptr && "Empty building group found while reassigning workers");

		std::set<BuildingType>::const_iterator it, end;

		for (it = buildingTypes->cbegin(), end = buildingTypes->cend(); it != end; ++it)
		{
			// Grupo i = groupPriority. Tipo j = (*it)
			std::set<CPlaceable*>* buildings = CBuildingManager::getSingletonPtr()->getBuildingsFromType(*it);
			if (buildings == nullptr)
				continue;

			std::set<CPlaceable*>::const_iterator itBuildings, endBuildings;

			for (itBuildings = buildings->cbegin(), endBuildings = buildings->cend(); itBuildings != endBuildings; ++itBuildings)
			{
				// Grupo i = groupPriority. Tipo j = (*it). Edificio k = (*itBuildings)
				CWorkBuilding *building = (*itBuildings)->getEntity()->getComponent<CWorkBuilding>();

				if (building != nullptr)
					buildingsFromGroup.push_back(building);
			}
		}

		return buildingsFromGroup;

	} // getBuildingsFromGroup

	//--------------------------------------------------------

	void CWorkManager::reassignWorkers()
	{
		// Recorremos en orden la lista de prioridades comprobando en qué edificios
		// se necesitan trabajadores para llegar al mínimo y, en ese caso, se los
		// asignamos de los edificios de grupos con menor prioridad
		int idxCurrentGroup = 0;
		int idxLessPriorityGroup = NUM_BUILDING_GROUPS - 1;

		while (idxCurrentGroup <= idxLessPriorityGroup)
		{
			std::vector<CWorkBuilding*> currentGroupBuildings = getBuildingsFromGroup(idxCurrentGroup);

			std::vector<CWorkBuilding*>::const_iterator it, end;

			for (it = currentGroupBuildings.cbegin(), end = currentGroupBuildings.cend(); it != end; ++it)
			{
				CWorkBuilding *currentBuilding = (*it);

				int assignedWorkers = currentBuilding->getAssignedWorkers();
				int minWorkers = currentBuilding->getMinWorkers();

				if (assignedWorkers < minWorkers)
				{
					// Edificio no funcional. Asignar trabajadores de edificios con
					// menor prioridad...
					bool enoughWorkers =
						relocateWorkers(currentBuilding, idxCurrentGroup, idxLessPriorityGroup, minWorkers - assignedWorkers);

					if (!enoughWorkers)
						break;
				}
			}

			++idxCurrentGroup;
		}

		// @TODO Posible mejora: Ir guardando referencias a los edificios cuyo número
		// de trabajadores asignados es superior al mínimo, para intentar llenar algún
		// edificio más de los menos prioritarios

	} // reassignWorkers

	//--------------------------------------------------------

	bool CWorkManager::relocateWorkers(CWorkBuilding* building, int groupPriority, int& lastPriorityGroup, int numWorkers)
	{
		// Comprobamos que no haya habido ningún tipo de error en la llamada al método
		if (groupPriority > lastPriorityGroup || numWorkers <= 0)
			return false;

		int numWorkersFound = 0;

		while (groupPriority <= lastPriorityGroup)
		{
			std::vector<CWorkBuilding*> buildingsFromLastGroup = getBuildingsFromGroup(lastPriorityGroup);

			std::vector<CWorkBuilding*>::const_reverse_iterator rit = buildingsFromLastGroup.crbegin();
			std::vector<CWorkBuilding*>::const_reverse_iterator rend = buildingsFromLastGroup.crend();

			for (; rit != rend; ++rit)
			{
				CWorkBuilding *currentBuilding = (*rit);

				// Si estamos recorriendo el mismo grupo de edificios que aquel para el que buscamos
				// trabajadores, paramos al encontrarnos con el edificio dado
				if (groupPriority == lastPriorityGroup && currentBuilding == building)
					return false;

				int buildingWorkers = currentBuilding->getAssignedWorkers();

				if (buildingWorkers > 0)
				{
					// Recoloca a tantos trabajadores como tenga el edificio (o como sigan siendo
					// necesarios)
					int numNewWorkers = std::min(buildingWorkers, numWorkers - numWorkersFound);
					if (!moveWorkers(building, currentBuilding, numNewWorkers))
						continue;

					numWorkersFound += numNewWorkers;

					if (numWorkersFound >= numWorkers)
						return true;
				}
			}

			--lastPriorityGroup;
		}
		
		return false;

	} // relocateWorkers

	//--------------------------------------------------------

	bool CWorkManager::moveWorkers(CWorkBuilding* targetBuilding, CWorkBuilding* sourceBuilding, int numWorkers)
	{
		CEntity *targetEntity = targetBuilding->getEntity();

		AI::CWorkTask *workTask = new AI::CWorkTask(targetEntity->getMap(), targetEntity->getEntityID(),
			CSoulsTrialManager::SoulsCategory::UNKNOWN);

		SoulSenderMessage m(workTask, numWorkers);
		bool result = m.Dispatch(*sourceBuilding->getEntity());

		// Si se ha podido enviar las almas a trabajar al edificio de destino, decrementamos
		// el número de trabajadores del de origen
		if (result)
			sourceBuilding->decrementAssignedWorkers(numWorkers);

		return result;

	} // moveWorkers

} // namespace Logic
