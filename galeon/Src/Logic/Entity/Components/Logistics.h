#ifndef LOGISTICS_H_
#define LOGISTICS_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMLogistics.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CLogistics : public CStateMachineExecutor<AI::CSMLogisticsData> {
		RTTI_DECL;
		DEC_FACTORY(CLogistics);

	public:
		/**
		Constructor por defecto.
		*/
		CLogistics() {}

		virtual ~CLogistics() {}

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
		AI::CStateMachine<AI::CLatentAction, AI::CSMLogisticsData>* getStateMachine(){
			return new AI::CSMLogistics(_entity);
		}

	}; // class CLogistics

	REG_FACTORY(CLogistics);

} // namespace Logic

#endif // LOGISTICS_H_