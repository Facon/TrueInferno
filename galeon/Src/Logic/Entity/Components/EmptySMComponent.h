/*
#ifndef EMPTY_SM_COMPONENT_
#define EMPTY_SM_COMPONENT_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMEmptySMComponent.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CEmptySMComponent : public CStateMachineExecutor<AI::CSMEmptySMComponentData> {
		RTTI_DECL;
		DEC_FACTORY(CEmptySMComponent);

	public:
		/**
		Constructor por defecto.
		/
		CEmptySMComponent() {}

		virtual ~CEmptySMComponent() {}

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		/
		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMEmptySMComponentData>* getStateMachine(){
			return new AI::CSMEmptySMComponent(_entity);
		}

	}; // class CEmptySMComponent

	REG_FACTORY(CEmptySMComponent);

} // namespace Logic

#endif // EMPTY_SM_COMPONENT_
*/