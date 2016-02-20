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
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Gesti�n de mensajes para enviar y recibir informaci�n de rutas de almas caminantes */
		virtual bool HandleMessage(const WalkSoulPathMessage& msg);

		/** Gesti�n de mensajes para solicitar acciones en el HellQuarters */
		virtual bool HandleMessage(const HellQuartersActionMessage& msg);

		/** Interfaz p�blica para que se solicite el env�o de un alma a trabajar */
		void requestSendSoulToWork();

		/** Interfaz p�blica para que se solicite el env�o de un alma a arder */
		void requestSendSoulToBurn();

	private:
		// Tiempo (ms) entre generaci�n de almas
		unsigned int _timeBetweenSpawns;

		// Cantidad de almas generadas
		unsigned int _numSpawnedSouls;

		// Tiempo (ms) desde la �litma generaci�n de almas
		unsigned int _timeSinceLastSpawn;

		// N�mero de almas disponibles
		int _numAvailableSouls;

		// N�mero de almas que se han solicitado enviar a trabajar
		int _numSoulsToWork;

		// N�mero de almas que se han solicitado enviar a arder
		int _numSoulsToBurn;

		// Estado del flujo de env�o de almas a trabajar
		SendingSoulToWorkState _sendingSoulToWorkState;

		std::vector<Vector3>* _pathReceived;

		/** Gestiona la l�gica del spawn de almas */
		void tickSpawnSouls(unsigned int msecs);

		/** Gestiona la l�gica del env�o de almas a trabajar */
		void tickSendSoulToWork(unsigned int msecs);

		/** Crea y env�a un alma a trabajar */
		bool CHellQuarters::createAndSendSoulToWork();

	}; // class CHellQuarters

	REG_FACTORY(CHellQuarters);

} // namespace Logic

#endif // HELL_QUARTERS_H_