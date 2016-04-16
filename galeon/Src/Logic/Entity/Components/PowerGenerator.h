#ifndef POWER_GENERATOR_H_
#define POWER_GENERATOR_H_

#include "StateMachineExecutor.h"

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"

#include "AI/SMPowerGenerator.h"
#include "AI/LatentAction.h"

namespace Logic {
	class CPowerGenerator : public CStateMachineExecutor<AI::CSMPowerGeneratorData> {
		RTTI_DECL;
		DEC_FACTORY(CPowerGenerator);

	public:
		/**
		Constructor por defecto.
		*/
		CPowerGenerator() {}

		virtual ~CPowerGenerator() {}

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Devuelve las unidades de consumo actual del generador */
		int getConsumptionUnits();

		/** Devuelve el periodo (ms) de consumo actual del generador */
		int getConsumptionPeriod();

		/** Devuelve las reservas actuales de combustible para el generador */
		int getCurrentReserves();

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMPowerGeneratorData>* getStateMachine(){
			return new AI::CSMPowerGenerator(_entity);
		}

	}; // class CPowerGenerator

	REG_FACTORY(CPowerGenerator);

} // namespace Logic

#endif // POWER_GENERATOR_H_