//---------------------------------------------------------------------------
// SoulManager.cpp
//---------------------------------------------------------------------------

/**
@file SoulManager.cpp

Contiene la implementación del gestor de almas.

@see Logic::CSoulManager

@author Raúl Segura
@date Abril, 2016
*/

#include "SoulManager.h"
#include <cassert>

namespace Logic {

	CSoulManager* CSoulManager::_instance = 0;

	//--------------------------------------------------------

	CSoulManager::CSoulManager()
	{
		_instance = this;
		_workers = std::map<CPlaceable*, std::set<CSoul*>*>();

	} // CSoulManager

	//--------------------------------------------------------

	CSoulManager::~CSoulManager()
	{
		assert(_instance);

		// Liberamos el índice de almas
		for (auto it = _workers.begin(); it != _workers.end(); ++it)
		{
			for (auto it2 = (*it).second->cbegin(); it2 != (*it).second->cend(); ++it2)
			{
				delete (*it2);
			}

			it->second->clear();

			delete it->second;
			it->second = nullptr;
		}

		_workers.clear();
		_instance = nullptr;

	} // ~CSoulManager

	//--------------------------------------------------------

	bool CSoulManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CSoulManager no permitida!");

		new CSoulManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CSoulManager::Release()
	{
		assert(_instance && "Logic::CSoulManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CSoulManager::registerWorker(CSoul *worker, CPlaceable *building)
	{
		// Comprobamos alma y edificio
		if (worker == nullptr || building == nullptr || !building->isBuilding())
			return;
		
		// Almacenamos en el mapa indexando por edificio:
		// Creamos, si no existe, el conjunto de trabajadores para el edificio dado
		std::set<CSoul*>* buildingWorkers = _workers[building];
		if (buildingWorkers == nullptr)
		{
			buildingWorkers = new std::set<CSoul*>();
			_workers[building] = buildingWorkers;
		}

		// Asignamos el trabajador al edificio
		worker->setBuilding(building);

		// Añadimos el trabajador
		buildingWorkers->insert(worker);

	} // registerWorker

	//--------------------------------------------------------

	void CSoulManager::unregisterWorker(CSoul *worker)
	{
		// Ignoramos almas no asignadas a un edificio
		CPlaceable *building = worker->getBuilding();
		if (building == nullptr || !building->isBuilding())
			return;

		// Eliminamos el puntero al trabajador del conjunto para su edificio
		std::set<CSoul*>* buildingWorkers = _workers[building];
		if (buildingWorkers != nullptr)
		{
			buildingWorkers->erase(worker);

			// Si era el último de su edificio, eliminamos el set
			if (buildingWorkers->empty())
				_workers.erase(building);

			delete buildingWorkers;
			buildingWorkers = nullptr;
		}

		// Eliminamos la referencia al edificio del trabajador
		worker->clearBuilding();

	} // unregisterWorker

	//--------------------------------------------------------

	void CSoulManager::changeWorkerBuilding(CSoul *worker, CPlaceable *newBuilding)
	{
		// Comprobamos alma y nuevo edificio antes de desregistrarla del antiguo
		if (worker == nullptr || newBuilding == nullptr || !newBuilding->isBuilding())
			return;

		unregisterWorker(worker);
		registerWorker(worker, newBuilding);

	} // changeWorkerBuilding

	//--------------------------------------------------------

	std::set<CSoul*>* CSoulManager::getBuildingWorkers(CPlaceable *building)
	{
		if (building == nullptr || !building->isBuilding())
			return nullptr;

		return _workers[building];

	} // getBuildingWorkers

	//--------------------------------------------------------

	void CSoulManager::modifyMovementSpeed(float factor)
	{
		assert(0 < factor && "La velocidad de movimiento debe multiplicarse por un factor positivo");

		_realMovementSpeed = _movementSpeed;
		_movementSpeed *= factor;

	} // modifyMovementSpeed

	//--------------------------------------------------------

	void CSoulManager::restoreMovementSpeed()
	{
		_movementSpeed = _realMovementSpeed;

	} // restoreMovementSpeed

	//--------------------------------------------------------

	bool CSoulManager::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CSoulManager::close()
	{
	} // close

} // namespace Logic
