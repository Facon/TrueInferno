#include "WorkBuilding.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Message.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/LogicRequirement.h"

#include <iostream>
#include <cassert>

namespace Logic {
	RTTI_ROOT_IMPL(CWorkBuilding);
	IMP_FACTORY(CWorkBuilding);

	CWorkBuilding::CWorkBuilding() : IComponent(), _askReceived(false) {
	}

	bool CWorkBuilding::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
	
		assert(entityInfo->hasAttribute("minWorkers") && "minWorkers is not defined");
		_minWorkers = entityInfo->getIntAttribute("minWorkers");

		assert(entityInfo->hasAttribute("maxWorkers") && "maxWorkers is not defined");
		_maxWorkers = entityInfo->getIntAttribute("maxWorkers");
		
		_activeWorkers = 0;
		_assignedWorkers = 0;

		_changeActive = 0;
		_changeAssigned = 0;

		_askReceived = false;

		return true;
	} // spawn

	void CWorkBuilding::tick(unsigned int msecs){
		// Si había pendiente una orden de cambiar el número de trabajadores activos
		if (_changeActive != 0){
			// Guardamos la cantidad antigua de trabajadores activos
			int oldActive = _activeWorkers;
			
			// Aplicamos el cambio
			_activeWorkers += _changeActive;
			
			// Si estábamos por debajo del mínimo pero ahora lo superamos
			if ((oldActive < (int)_minWorkers) && (_activeWorkers >= _minWorkers)){
				// Eliminamos el requisito de trabajadores
				ToggleMessage m(LogicRequirement::Workers, false);
				bool result = m.Dispatch(*_entity);
				assert(result && "Can't remove 'Workers' requirement");
			}

			// Si estábamos por encima del mínimo pero ya no lo superamos
			else if ((oldActive >= (int)_minWorkers) && (_activeWorkers < _minWorkers)){
				// Añadimos el requisito de trabajadores
				ToggleMessage m(LogicRequirement::Workers, true);
				bool result = m.Dispatch(*_entity);
				assert(result && "Can't add 'Workers' requirement");
			}

			//std::cout << "Current active workers=" << _activeWorkers << std::endl;

			_changeActive = 0;
		}

		// Si había pendiente una orden de cambiar el número de trabajadores asignados
		if (_changeAssigned != 0){
			// Aplicamos el cambio
			_assignedWorkers += _changeAssigned;

			//std::cout << "Current assigned workers=" << _assignedWorkers << std::endl;

			_changeAssigned = 0;
		}

		// Si había petición de información
		if (_askReceived){
			WorkerMessage info;
			info.assembleWorkerInfo(_minWorkers, _maxWorkers, _activeWorkers, _assignedWorkers);

			// Reenviamos hasta que lo acepten
			if (info.Dispatch(*_entity))
				_askReceived = false;
		}

	} // tick

	bool CWorkBuilding::isActive(){
		return _activeWorkers >= _minWorkers;
	}

	bool CWorkBuilding::HandleMessage(const WorkerMessage& msg){
		switch(msg._type){
		case MessageType::WORKER_ACTIVATED:{
			_changeActive += msg._change;
			break;
		}

		case MessageType::WORKER_ASSIGNED:{
			_changeAssigned += msg._change;
			break;
		}

		case MessageType::WORKER_ASK:{
			_askReceived = true;
			break;
		}

		case MessageType::WORKER_INFO:{
			break;
		}

		default:{
			assert("Unmiplemented logic for message type" && false);
			return false;
		}
		}

		return true;
	}

	void CWorkBuilding::decrementAssignedWorkers(int numWorkers)
	{
		_assignedWorkers -= numWorkers;

		if (_assignedWorkers < 0)
			_assignedWorkers = 0;
	}

	// Ignoramos el requisito de los trabajadores
	void CWorkBuilding::defineSkippedRequirements()
	{
		_skippedRequirements.insert(LogicRequirement::Workers);
	}

} // namespace Logic