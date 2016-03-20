#include "LAExecuteResourceProvideTasks.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceProviderData.h"
#include <algorithm>
#include "Logic\Maps\Map.h"
#include "TransportSoulTask.h"

namespace AI {
	CLatentAction::LAStatus CLAExecuteResourceProvideTasks::OnStart() {
		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAExecuteResourceProvideTasks::OnRun(unsigned int msecs){
		std::cout << "Unimplemented logic for 'processBringResourcesTo'" << std::endl;
		return LAStatus::FAIL; // TODO
	}

}