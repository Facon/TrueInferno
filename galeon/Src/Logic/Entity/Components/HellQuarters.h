#ifndef HELL_QUARTERS_H_
#define HELL_QUARTERS_H_

#include "AI/SMHellQuarters.h"
#include "BaseSubsystems/RTTI.h"
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/StateMachineExecutor.h"
#include <vector>

namespace Logic {
	class CHellQuarters : public CStateMachineExecutor<AI::CSMHellQuartersData>{
		RTTI_DECL;
		DEC_FACTORY(CHellQuarters);

	public:
		/**
		Constructor por defecto.
		*/
		CHellQuarters() {};

		virtual ~CHellQuarters() {};

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMHellQuartersData>* getStateMachine(){
			return new AI::CSMHellQuarters(_entity);
		}

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_