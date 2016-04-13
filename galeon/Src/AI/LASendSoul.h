#ifndef LA_SEND_SOUL_
#define LA_SEND_SOUL_

#include "LatentAction.h"
#include "Server.h"
#include "SoulTask.h"
#include "Logic\Entity\Message.h"

namespace AI {
	class CLASendSoul : public CLatentAction {
		RTTI_DECL;

	public:
		CLASendSoul(CEntity* entity) : CLatentAction(entity) {
			_task = nullptr;
		}

		virtual ~CLASendSoul() {
			if (_task){
				delete _task;
				_task = nullptr;
			}
		}

		virtual bool HandleMessage(const SoulSenderMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		const float SOUL_ON_TILE_HEIGHT = 2.0;

		//std::unique_ptr<AI::CSoulTask> _task;

		/* Tarea a asignar a las almas */
		AI::CSoulTask* _task;

		/* Número de almas que se enviarán a la tarea */
		int _numSouls;

		/* Vector con las nuevas almas creadas */
		std::vector<CEntity*> _newSouls;

		/* Número de almas ya enviadas */
		int _numSoulsSent;

		/* Crea las almas. Devuelve true cuando se acabó de crear todas las almas */
		bool createSouls();

		/* Envía a las almas a su misión. Devuelve true cuando se acabó de enviar todas las almas */
		bool sendSouls();
	};

}

#endif