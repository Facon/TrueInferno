#ifndef RESOURCE_DEMANDER_H_
#define RESOURCE_DEMANDER_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMResourceDemander.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CResourceDemander : public CStateMachineExecutor<AI::CSMResourceDemanderData> {
		RTTI_DECL;
		DEC_FACTORY(CResourceDemander);

	public:
		/**
		Constructor por defecto.
		*/
		CResourceDemander() {}

		virtual ~CResourceDemander() {}

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMResourceDemanderData>* getStateMachine(){
			return new AI::CSMResourceDemander(_entity);
		}

	}; // class CResourceDemander

	REG_FACTORY(CResourceDemander);

} // namespace Logic

#endif // RESOURCE_DEMANDER_H_