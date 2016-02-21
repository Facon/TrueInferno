#ifndef SOUL_SENDER_H_
#define SOUL_SENDER_H_

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
#include <vector>

namespace Logic {
	struct SoulSendRequest{
		AI::CSoulTask task;
		int _numSouls;
	};

	class CSoulSender : public IComponent{
		RTTI_DECL;
		DEC_FACTORY(CSoulSender);

		enum SendingSoulState{
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
		CSoulSender();

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

		/** Gestión de mensajes para solicitar envíos de almas */
		virtual bool HandleMessage(const SoulSenderRequestMessage& msg);

		/** Interfaz pública para que se solicite el envío de un alma */
		void requestSendSoulToSend();

	private:
		static const float SOUL_ON_TILE_HEIGHT;

		// Número de almas disponibles
		int _numAvailableSouls;

		// Número de almas que se han solicitado enviar
		int _numSoulsToSend;

		// Estado del flujo de envío de almas a trabajar
		SendingSoulState _sendingSoulState;

		std::vector<Vector3>* _pathReceived;

		/** Crea y envía un alma a trabajar */
		bool CSoulSender::createAndSendSoulToWork();

	}; // class CSoulSender

	REG_FACTORY(CSoulSender);

} // namespace Logic

#endif // SOUL_SENDER_H_