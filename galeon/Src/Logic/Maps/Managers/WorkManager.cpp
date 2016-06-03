//---------------------------------------------------------------------------
// WorkManager.cpp
//---------------------------------------------------------------------------

/**
@file WorkManager.cpp

Contiene la implementaci�n del gestor de trabajo.

@see Logic::CWorkManager

@author �lvaro Valera, Ra�l Segura
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
		assert(!_instance && "Segunda inicializaci�n de Logic::CWorkManager no permitida!");

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
		assert(_instance && "Logic::CWorkManager no est� inicializado!");

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

	TEntityID CWorkManager::findBuildingToWork()
	{
		CWorkBuilding *nonFullBuilding = nullptr;

		// Buscaremos, en el orden de prioridad definido para los diferentes
		// grupos de edificios, el primero cuyo n�mero de trabajadores
		// asignados no llegue al m�nimo necesario
		for (unsigned int group = 0; group < NUM_BUILDING_GROUPS; ++group)
		{
			std::vector<CWorkBuilding*> currentGroupBuildings = getBuildingsFromGroup(group);

			std::vector<CWorkBuilding*>::const_iterator it, end;

			for (it = currentGroupBuildings.cbegin(), end = currentGroupBuildings.cend(); it != end; ++it)
			{
				CWorkBuilding *currentBuilding = *it;

				if (currentBuilding->getVirtualAssignedWorkers() < currentBuilding->getMinWorkers())
					return currentBuilding->getEntity()->getEntityID();

				// Al mismo tiempo que iteramos sobre los edificios buscando uno que no llegue
				// al m�nimo n�mero de trabajadores, nos guardamos el primero que no est�
				// completo (con lo que nos ahorramos volver a iterar en caso de que todos
				// lleguen al m�nimo)
				if (nonFullBuilding == nullptr &&
					currentBuilding->getVirtualAssignedWorkers() < currentBuilding->getMaxWorkers())
					nonFullBuilding = currentBuilding;
			}
		}

		// Todos los edificios llegan al m�nimo de trabajadores...
		return (nonFullBuilding != nullptr) ? nonFullBuilding->getEntity()->getEntityID() : EntityID::UNASSIGNED;

	} // findBuildingToWork

	//--------------------------------------------------------

	TEntityID CWorkManager::findFurnace()
	{
		// Buscaremos el horno con mayor capacidad actual para quemar almas
		TEntityID target = EntityID::UNASSIGNED;
		int maxSoulsToBurn = INT_MIN;

		// Obtenemos los hornos (si hay alguno)
		std::set<CPlaceable*>* _furnaces = CBuildingManager::getSingletonPtr()->getBuildingsFromType(BuildingType::Furnace);
		
		if (_furnaces == nullptr)
			return EntityID::UNASSIGNED;

		for (auto it = _furnaces->cbegin(); it != _furnaces->cend(); ++it)
		{
			CEntity* entity = (*it)->getEntity();

			// Los hornos deben ser edificios de trabajadores
			CWorkBuilding* furnace = entity->getComponent<CWorkBuilding>();
			if (furnace == nullptr)
			{
				assert(false && "Every furnace must have a WorkBuilding component");
				continue;
			}

			// Ignoramos los edificios no activos
			if (!furnace->isActive())
				continue;

			// Los hornos deben ser quemadores de almas
			CSoulBurner* soulBurner = entity->getComponent<CSoulBurner>();
			if (soulBurner == nullptr)
			{
				assert(false && "Every furnace must have a SoulBurner component");
				continue;
			}

			// Obtenemos la capacidad restante del horno
			int remainingSoulsToBurn = soulBurner->getMaxSoulsToBurn() - soulBurner->getCurrentSoulsToBurn();

			// Actualizaremos el horno seleccionado si...
			// ...se mejora la mayor capacidad encontrada hasta el momento
			if (remainingSoulsToBurn > maxSoulsToBurn)
			{
				maxSoulsToBurn = remainingSoulsToBurn;
				target = entity->getEntityID();
			}

			// TODO ...o si se iguala y est� m�s cerca
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
		// NUM_BUILDING_GROUPS, y que �sta es �nica
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
		// Recorremos en orden la lista de prioridades comprobando en qu� edificios
		// se necesitan trabajadores para llegar al m�nimo y, en ese caso, se los
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

				int assignedWorkers = currentBuilding->getVirtualAssignedWorkers();
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

		// @TODO Posible mejora: Ir guardando referencias a los edificios cuyo n�mero
		// de trabajadores asignados es superior al m�nimo, para intentar llenar alg�n
		// edificio m�s de los menos prioritarios

	} // reassignWorkers

	//--------------------------------------------------------

	bool CWorkManager::relocateWorkers(CWorkBuilding* building, int groupPriority, int& lastPriorityGroup, int numWorkers)
	{
		// Comprobamos que no haya habido ning�n tipo de error en la llamada al m�todo
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

				int buildingWorkers = currentBuilding->getVirtualAssignedWorkers();

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
			SoulsTrialManager::SoulsCategory::UNKNOWN);

		// Esto evita que se incremente el n�mero de trabajadores asignados al edificio de
		// destino en el m�todo start() de la tarea, ya que se hace aqu� mismo de forma
		// s�ncrona
		workTask->setWorkerAssigned(true);

		SoulSenderMessage m(workTask, numWorkers);
		bool result = m.Dispatch(*sourceBuilding->getEntity());

		// Si se ha podido enviar las almas a trabajar al edificio de destino, decrementamos
		// el n�mero de trabajadores del de origen y forzamos el incremento en el primero
		if (result)
		{
			sourceBuilding->decreaseAssignedWorkers(numWorkers);
			targetBuilding->increaseAssignedWorkers(numWorkers);
		}

		return result;

	} // moveWorkers

} // namespace Logic
