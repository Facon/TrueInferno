#include "LAExecuteLogisticsTasks.h"

#include "Logic\Entity\Message.h"
#include "AI\SMLogisticsData.h"
#include <algorithm>
#include "Logic\Maps\Map.h"
#include "TransportSoulTask.h"

namespace AI {
	// Objeto para ordenar por distancia a un punto fijo
	struct st_sorterByDistance{
		Vector3 pos;
		CMap *map;

		bool operator() (ResourceMessage i, ResourceMessage j) {
			Logic::CEntity* iEntity = map->getEntityByID(i._caller);
			if (iEntity == nullptr)
				return false; // i es null ==> j antes que i

			Logic::CEntity* jEntity = map->getEntityByID(j._caller);
			if (jEntity == nullptr)
				return true; // j es null ==> i antes que j

			return (pos.squaredDistance(iEntity->getPosition()) < pos.squaredDistance(jEntity->getPosition()));
		}

	} sorterByDistance;

	CLatentAction::LAStatus CLAExecuteLogisticsTasks::OnStart() {
		// Obtenemos los mensajes de proveedores
		std::vector<ResourceMessage> providerMessages = _smData.getProviderMessages();

		// Los ordenamos por distancia a nuestra posici�n
		sorterByDistance.pos = _entity->getPosition();
		sorterByDistance.map = _entity->getMap();
		std::sort(providerMessages.begin(), providerMessages.end(), sorterByDistance);
	
		// Para cada uno vamos solicitando lo que tengan hasta completar nuestras necesidades
		unsigned int remaining = _smData.getResourceQuantity();
		for (auto it = providerMessages.cbegin(); it != providerMessages.cend(); ++it){
			// Finalizamos si ya solicitamos todo lo posible
			if (_smData.getResourceQuantity() == 0)
				break;

			// Nos aseguramos de no sobrepasar lo que nos queda por solicitar ni lo disponible en el proveedor
			unsigned int request = std::min(_smData.getResourceQuantity(), it->_available);

			/** Enviamos la petici�n de transporte de recursos desde el proveedor hacia nosotros.
			El alma se crear� en la ubicaci�n actual, viajar� hacia el proveedor y volver� con los recursos */
			SoulSenderMessage msg(new CTransportTask(_entity->getMap(), it->_caller, _entity->getEntityID(), it->_resourceType, request), 1);
			
			// Si el mensaje es aceptado reducimos la cantidad restante por solicitar
			if(msg.Dispatch(*_entity))
				remaining -= request;
		}

		// Si no qued� nada por solicitar tuvimos �xito
		if (remaining == 0)
			return LAStatus::SUCCESS;
		else
			return LAStatus::FAIL;
	}

}