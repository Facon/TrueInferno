#ifndef HELL_QUARTERS_H_
#define HELL_QUARTERS_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Message.h"
#include <vector>

namespace Logic {
	enum HellQuartersAction {
		BurnSoul,
		SendSoulToWork
	};

	enum HellQuartersActionState{
		Idle,
		Requested,
		WaitingTask,
		Fail,
		Success,
		Clean
	};

	class CHellQuarters : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CHellQuarters);

	public:
		/**
		Constructor por defecto.
		*/
		CHellQuarters();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualización por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Gestión de mensajes para solicitar acciones en el HellQuarters */
		virtual bool HandleMessage(const HellQuartersActionMessage& msg);

		/** Gestión de mensajes para recoger respuestas del SoulSender */
		virtual bool HandleMessage(const SoulSenderResponseMessage& msg);
			
	private:
		static const float SOUL_ON_TILE_HEIGHT;

		// Tiempo (ms) entre generación de almas
		unsigned int _timeBetweenSpawns;

		// Cantidad de almas generadas
		unsigned int _numSpawnedSouls;

		// Tiempo (ms) desde la úlitma generación de almas
		unsigned int _timeSinceLastSpawn;

		// Número de almas disponibles
		int _numAvailableSouls;

		/** Gestiona la lógica del spawn de almas */
		void tickSpawnSouls(unsigned int msecs);

		/** Gestiona la lógica de acciones solicitadas */
		void tickActions(unsigned int msecs);

		HellQuartersActionState _hellQuartersActionState;

		HellQuartersActionMessage* _actionRequested;

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_