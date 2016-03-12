#include "LAExecuteLogisticsTasks.h"

#include "Logic\Entity\Message.h"
#include "AI\SMLogisticsData.h"
#include <algorithm>

namespace AI {
	// Objeto para ordenar por distancia a un punto fijo
	struct st_sorterByDistance{
		Vector3 pos;

		bool sortByDistance(LogisticsMessage i, LogisticsMessage j) {
			//return (pos.squaredDistance(i._target) < pos.squaredDistance(j._target));
			return true; // TODO
		}

	} sorterByDistance;

	CLatentAction::LAStatus CLAExecuteLogisticsTasks::OnStart() {
		// Obtenemos los mensajes de proveedores
		std::vector<LogisticsMessage> providerMessages = _smData.getProviderMessages();

		// TODO Los ordenamos por distancia a nuestra posición
		sorterByDistance.pos = _entity->getPosition();
		//std::sort(providerMessages.begin(), providerMessages.end(), sorterByDistance);

		// TODO
		return LAStatus::SUCCESS;
	}

	CLatentAction::LAStatus CLAExecuteLogisticsTasks::OnRun(unsigned int msecs) {
		return LAStatus::SUCCESS;
	}

}