#include "LAReserveFromProviders.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceDemanderData.h"

namespace AI {

	CLatentAction::LAStatus CLAReserveFromProviders::OnStart() {
		// Recorremos los mensajes de los proveeodres
		for (auto it = _smData.getProviderMessages().begin(); it != _smData.getProviderMessages().end(); ++it){
			// E intentamos reservar todo lo que podamos de cada uno
			// TODO
		}
		return LAStatus::SUCCESS;
	}

}