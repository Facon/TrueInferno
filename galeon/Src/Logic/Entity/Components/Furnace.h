#ifndef FURNACE_H_
#define FURNACE_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Message.h"
#include <vector>

namespace Logic {

	enum FurnaceActionState {
		Idle,
		Requested,
		WaitingTask,
		Fail,
		Success,
		Clean
	};

	class CFurnace : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CFurnace);

	public:
		/**
		Constructor por defecto.
		*/
		CFurnace();

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Actualizaci�n por frame
		*/
		virtual void tick(unsigned int msecs);

		/** Gesti�n de mensajes para solicitar acciones en el Furnace */
		virtual bool HandleMessage(const FurnaceActionMessage& msg);

		/** Gesti�n de mensajes para recoger respuestas del SoulSender */
		virtual bool HandleMessage(const SoulSenderResponseMessage& msg);
			
	private:
		static const float SOUL_ON_TILE_HEIGHT;

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

		/** Gestiona la l�gica de acciones solicitadas */
		void tickActions(unsigned int msecs);

		FurnaceActionState _furnaceActionState;

		FurnaceActionMessage* _actionRequested;

	}; // class CFurnace

	REG_FACTORY(CFurnace);

} // namespace Logic

#endif // FURNACE_H_