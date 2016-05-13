#include "LAStartSoulTask.h"

#include "AI/SoulTask.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Graphics\Entity.h"

namespace AI {
	RTTI_IMPL(CLAStartSoulTask, CLatentAction);
	
	CLatentAction::LAStatus CLAStartSoulTask::OnStart() {
		// Intentamos empezar la tarea
		// TODO Cambiar bool por enumerado para abortar en caso de fallo. Actualmente los fallos no reintentables nos están devolviendo true!
		if (_smData.getTask()->start()){
			// Sacamos entidad gráfica
			CMap* map = Logic::CServer::getSingletonPtr()->getMap();
			CEntity* entity = map->getEntityByID(_smData.getTask()->getTarget());
			Graphics::CEntity* graphics = entity->getComponent<CGraphics>()->getGraphicsEntity();

			// Pasamos el nuevo color a la entidad gráfica del alma
			_entity->getComponent<CGraphics>()->getGraphicsEntity()->setColor(graphics->getColor());

			return LAStatus::SUCCESS;
		}

		// Si no se pudo arrancar esperamos al siguiente tick
		else
			return LAStatus::READY;
	}
}