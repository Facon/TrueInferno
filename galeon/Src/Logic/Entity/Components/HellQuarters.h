#ifndef HELL_QUARTERS_H_
#define HELL_QUARTERS_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
#include <vector>

namespace Logic {
	class CHellQuarters : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CHellQuarters);

		enum SendingSoulToWorkState{
			Idle,
			WaitingForPath,
			PathReceived
		};

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

		/** Gestión de mensajes para enviar y recibir información de rutas de almas caminantes */
		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

	private:
		// Tiempo (ms) entre generación de almas
		unsigned int _timeBetweenSpawns;

		// Cantidad de almas generadas
		unsigned int _numSpawnedSouls;

		// Tiempo (ms) desde la úlitma generación de almas
		unsigned int _timeSinceLastSpawn;

		// Número de almas disponibles
		int _numAvailableSouls;

		// Flag a true cuando se ha solicitado enviar almas a trabajar
		bool _soulsToWorkRequest;

		// Flag a true cuando se ha solicitado enviar almas a quemarse
		bool _soulsToBurnRequest;

		/** Genera almas periódicamente */
		void spawnSouls(unsigned int msecs);

		/** Send a soul to work */
		bool sendSoulToWork();

		SendingSoulToWorkState _sendingSoulToWorkState;

		std::vector<Vector3>* _pathReceived;

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_