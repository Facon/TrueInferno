#include "LAGetWalkingSoulPath.h"

namespace AI {
	CLatentAction::LAStatus CLAGetWalkingSoulPath::OnStart() {
		// Inicializamos la ruta a null
		_smData.setPath(nullptr);

		// Indicamos que no hemos recibido mensaje de ruta
		_pathMessageReceived = false;

		// Intentamos o reintentamos la petici�n de ruta
		WalkSoulPathMessage m(_smData.getTask()->getTarget());
		if (m.Dispatch(*_entity)){
			// Si hubo �xito nos dormimos a la espera de que llegue la ruta
			return LAStatus::SUSPENDED;
		}
		else
			return LAStatus::READY;
	}

	bool CLAGetWalkingSoulPath::HandleMessage(const WalkSoulPathMessage& msg) {
		// Rechazamos lo que no sean mensajes con la ruta
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH)
			return false;

		//No aceptamos m�s de una ruta simult�neamente
		if (_pathMessageReceived)
			return false;

		// Guardamos la ruta
		_smData.setPath(msg._path);

		// Indicamos que recibimos el mensaje
		_pathMessageReceived = true;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAGetWalkingSoulPath::OnRun(unsigned int msecs) {
		// Si la ruta obtenida es nula fallamos
		if (_smData.getPath() == nullptr)
			return LAStatus::FAIL;

		// �xito en otro caso
		else
			return LAStatus::SUCCESS;
	}

}