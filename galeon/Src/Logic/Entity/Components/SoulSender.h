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

		/** Gesti�n de mensajes para solicitar env�os de almas */
		virtual bool HandleMessage(const SoulSenderRequestMessage& msg);

		/** Interfaz p�blica para que se solicite el env�o de un alma */
		void requestSendSoulToSend();

	private:
		static const float SOUL_ON_TILE_HEIGHT;

		// N�mero de almas disponibles
		int _numAvailableSouls;

		// N�mero de almas que se han solicitado enviar
		int _numSoulsToSend;

		// Estado del flujo de env�o de almas a trabajar
		SendingSoulState _sendingSoulState;

		std::vector<Vector3>* _pathReceived;

		/** Crea y env�a un alma a trabajar */
		bool CSoulSender::createAndSendSoulToWork();

	}; // class CSoulSender

	REG_FACTORY(CSoulSender);

} // namespace Logic

#endif // SOUL_SENDER_H_