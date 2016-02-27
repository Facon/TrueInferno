#ifndef SOUL_SENDER_H_
#define SOUL_SENDER_H_

#include "Logic/Entity/Component.h"
#include "StateMachineExecutor.h"
#include "AI/SMSoulSender.h"

namespace Logic {
	class CSoulSender : public CStateMachineExecutor {
		RTTI_DECL;
		DEC_FACTORY(CSoulSender);

	public:
		/**
		Constructor por defecto.
		*/
		CSoulSender() {};

		virtual ~CSoulSender() {};

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
		AI::CStateMachine<AI::CLatentAction>* getStateMachine(){
			return new AI::CSMSoulSender(_entity);
		}

	}; // class CSoulSender

	REG_FACTORY(CSoulSender);

} // namespace Logic

#endif // SOUL_SENDER_H_