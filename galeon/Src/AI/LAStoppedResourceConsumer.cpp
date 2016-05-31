#include "LAStoppedResourceConsumer.h"

#include "Logic\Entity\IconType.h"
#include "Logic/Entity/Components/Billboard.h"

namespace AI {
	RTTI_IMPL(CLAStoppedResourceConsumer, CLatentAction);
	
	CLatentAction::LAStatus CLAStoppedResourceConsumer::OnStart() {
		// Mostramos el icono de necesidad de recurso
		/* Nota: Mmarcando aquí la necesidad de recurso desde el inicio del juego nos estaría avisando aunque no haya edificios conectados
		IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(_consumedResource));
		const bool result = m.Dispatch(*_entity);
		assert(result && "Can't set resource icon");
		*/

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAStoppedResourceConsumer::OnRun(unsigned int msecs) {
		return LAStatus::RUNNING;
	}

}