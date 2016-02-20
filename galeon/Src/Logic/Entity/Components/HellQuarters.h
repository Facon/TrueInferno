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
			Requested,
			WaitingForPath,
			PathReceived,
			Success,
			Fail,
			Clean
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

		/** Gestión de mensajes para solicitar acciones en el HellQuarters */
		virtual bool HandleMessage(const HellQuartersActionMessage& msg);

		/** Interfaz pública para que se solicite el envío de un alma a trabajar */
		void requestSendSoulToWork();

		/** Interfaz pública para que se solicite el envío de un alma a arder */
		void requestSendSoulToBurn();

	private:
		// Tiempo (ms) entre generación de almas
		unsigned int _timeBetweenSpawns;

		// Cantidad de almas generadas
		unsigned int _numSpawnedSouls;

		// Tiempo (ms) desde la úlitma generación de almas
		unsigned int _timeSinceLastSpawn;

		// Número de almas disponibles
		int _numAvailableSouls;

		// Número de almas que se han solicitado enviar a trabajar
		int _numSoulsToWork;

		// Número de almas que se han solicitado enviar a arder
		int _numSoulsToBurn;

		// Estado del flujo de envío de almas a trabajar
		SendingSoulToWorkState _sendingSoulToWorkState;

		std::vector<Vector3>* _pathReceived;

		/** Gestiona la lógica del spawn de almas */
		void tickSpawnSouls(unsigned int msecs);

		/** Gestiona la lógica del envío de almas a trabajar */
		void tickSendSoulToWork(unsigned int msecs);

		/** Crea y envía un alma a trabajar */
		bool CHellQuarters::createAndSendSoulToWork();

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_