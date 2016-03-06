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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

	protected:
		AI::CStateMachine<AI::CLatentAction, AI::CSMHellQuartersData>* getStateMachine(){
			return new AI::CSMHellQuarters(_entity);
		}

	private:
		// Tiempo (ms) entre generaci�n de almas
		unsigned int _timeBetweenSpawns;

		// Cantidad de almas generadas
		unsigned int _numSpawnedSouls;

		// Tiempo (ms) desde la �litma generaci�n de almas
		unsigned int _timeSinceLastSpawn;

		// N�mero de almas disponibles
		int _numAvailableSouls;

		/** Gestiona la l�gica del spawn de almas */
		void tickSpawnSouls(unsigned int msecs);

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_