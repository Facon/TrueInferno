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

		bool sortByDistance(LogisticsMessage i, LogisticsMessage j) {
			Logic::CEntity* iEntity = map->getEntityByID(i._target);
			if (iEntity == nullptr)
				return false; // i es null ==> j antes que i

			Logic::CEntity* jEntity = map->getEntityByID(j._target);
			if (jEntity == nullptr)
				return true; // j es null ==> i antes que j

			return (pos.squaredDistance(iEntity->getPosition()) < pos.squaredDistance(jEntity->getPosition()));
		}

	} sorterByDistance;

	CLatentAction::LAStatus CLAExecuteLogisticsTasks::OnStart() {
		// Obtenemos los mensajes de proveedores
		std::vector<LogisticsMessage> providerMessages = _smData.getProviderMessages();

		// Los ordenamos por distancia a nuestra posición
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
			unsigned int request = std::min(_smData.getResourceQuantity(), it->_resourceQuantity);

			// Enviamos la petición
			SoulSenderMessage msg(new CTransportTask(it->_target), 1);
			
			remaining -= request;
		}
		
		// TODO
		return LAStatus::SUCCESS;
	}

	CLatentAction::LAStatus CLAExecuteLogisticsTasks::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}