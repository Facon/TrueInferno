#include "LAGetWalkingSoulPath.h"

#include "AI/SoulTask.h"
#include "Logic\Entity\Components\Billboard.h"
#include "Logic\Maps\Managers\TileManager.h"
#include "Logic\Entity\Components\Tile.h"

namespace AI {
	RTTI_IMPL(CLAGetWalkingSoulPath, CLatentAction);
	
	CLatentAction::LAStatus CLAGetWalkingSoulPath::OnStart() {
		// Inicializamos la ruta a null
		_smData.setPath(nullptr);

		// Indicamos que no hemos recibido mensaje de ruta
		_pathMessageReceived = false;

		// Intentamos o reintentamos la petición de ruta
		WalkSoulPathMessage m(_smData.getTask()->getTarget());
		if (m.Dispatch(*_entity)){
			// Si hubo éxito nos dormimos a la espera de que llegue la ruta
			return LAStatus::SUSPENDED;
		}
		else
			return LAStatus::READY;
	}

	bool CLAGetWalkingSoulPath::HandleMessage(const WalkSoulPathMessage& msg) {
		// Rechazamos lo que no sean mensajes con la ruta
		if (msg._type != MessageType::RETURN_WALK_SOUL_PATH)
			return false;

		//No aceptamos más de una ruta simultáneamente
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
		// Gestionamos los iconos a mostrar/ocultar de carretera en el edificio origen y destino
		CEntity* fromBuilding = nullptr;
		Logic::Tile* tile = Logic::CTileManager::getSingletonPtr()->getNearestTile(_entity->getPosition());
		if (tile != nullptr && tile->getPlaceableAbove() != nullptr)
			fromBuilding = tile->getPlaceableAbove()->getEntity();

		TEntityID toBuildingID = _smData.getTask()->getTarget();
		CEntity* toBuilding = _entity->getMap()->getEntityByID(toBuildingID);

		// Si la ruta obtenida es nula fallamos
		if (_smData.getPath() == nullptr) {
			// Si es posible, marcamos que necesitamos carretera en el origen
			if (fromBuilding != nullptr){
				IconMessage m(MessageType::ICON_ADD, Billboard::getLogicRequirementIcon(LogicRequirement::Road));
				bool result = m.Dispatch(*fromBuilding);
				assert(result && "Can't add road icon");
			}

			// Si es posible, marcamos que necesitamos carretera en el destino
			if (toBuilding != nullptr){
				// Marcamos que necesitamos carretera
				IconMessage m(MessageType::ICON_ADD, Billboard::getLogicRequirementIcon(LogicRequirement::Road));
				bool result = m.Dispatch(*toBuilding);
				assert(result && "Can't add road icon");
			}

			return LAStatus::FAIL;
		}

		// Éxito en otro caso
		else {
			// Si es posible, eliminamos la necesidad de carretera en el origen
			if (fromBuilding != nullptr){
				IconMessage m(MessageType::ICON_DELETE, Billboard::getLogicRequirementIcon(LogicRequirement::Road));
				bool result = m.Dispatch(*fromBuilding);
				assert(result && "Can't delete road icon");
			}

			// Si es posible, eliminamos la necesidad de carretera en el destino
			if (toBuilding != nullptr){
				// Marcamos que necesitamos carretera
				IconMessage m(MessageType::ICON_DELETE, Billboard::getLogicRequirementIcon(LogicRequirement::Road));
				bool result = m.Dispatch(*toBuilding);
				assert(result && "Can't delete road icon");
			}

			return LAStatus::SUCCESS;
		}
	}

}