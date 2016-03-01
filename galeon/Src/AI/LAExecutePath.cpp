#include "LAExecutePath.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"

namespace AI {
	bool CLAExecutePath::HandleMessage(const WalkSoulPathMessage& msg) {
		// Rechazamos lo que no sean mensajes con la ruta. No aceptamos más de uno simultáneamente
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH || _path != nullptr)
			return false;

		_path = msg._path;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAExecutePath::OnStart() {
		// Inicializamos
		_path = nullptr;

		// Suspendemos la LA hasta que llegue el mensaje con la ruta
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAExecutePath::OnRun() {
		// Verificamos que hay ruta
		if (_path == nullptr)
			return LAStatus::FAIL;

		return sendPath() ? LAStatus::SUCCESS : LAStatus::FAIL;
	}

	bool CLAExecutePath::sendPath(){
		WalkSoulPathMessage m(_path);
		m._type = MessageType::PERFORM_WALK_SOUL_PATH;

		return m.Dispatch(*_entity);
	}
}