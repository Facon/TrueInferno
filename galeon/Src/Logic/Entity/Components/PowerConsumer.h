#ifndef POWER_CONSUMER_H_
#define POWER_CONSUMER_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMPowerConsumer.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CPowerConsumer : public CStateMachineExecutor<AI::CSMPowerConsumerData> {
		RTTI_DECL;
		DEC_FACTORY(CPowerConsumer);

	public:
		/**
		Constructor por defecto.
		*/
		CPowerConsumer() {}

		virtual ~CPowerConsumer() {}

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
		AI::CStateMachine<AI::CLatentAction, AI::CSMPowerConsumerData>* getStateMachine(){
			return new AI::CSMPowerConsumer(_entity);
		}

		virtual void CPowerConsumer::defineSkippedRequirements();

	}; // class CPowerConsumer

	REG_FACTORY(CPowerConsumer);

} // namespace Logic

#endif // POWER_CONSUMER_H_