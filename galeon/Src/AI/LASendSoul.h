#ifndef LA_SEND_SOUL_
#define LA_SEND_SOUL_

#include "LatentAction.h"
#include "Server.h"
#include "SoulTask.h"
#include "WorkSoulTask.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/EntityFactory.h"

namespace AI {
	class CLASendSoul : public CLatentAction {
		RTTI_DECL;

	public:
		CLASendSoul(CEntity* entity) : CLatentAction(entity), _newSouls() {
			_task = nullptr;
		}

		virtual ~CLASendSoul() {
			if (_task){
				delete _task;
				_task = nullptr;
			}
			
			_newSouls.clear();
		}

		virtual bool HandleMessage(const SoulSenderMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		const float SOUL_ON_TILE_HEIGHT = 2.0;

		/* Tarea a asignar a las almas */
		//std::unique_ptr<AI::CSoulTask> _task;
		AI::CSoulTask* _task;

		/* Número de almas que se enviarán a la tarea */
		int _numSouls;

		/* Número de almas ya enviadas */
		int _numSoulsSent;

		/* Tiempo transcurrido desde el último alma enviada */
		unsigned int _timeSinceLastSoulSent = 0;

		/* Tiempo a esperar entre cada alma enviada */
		unsigned int _timeBetweenSouls = 200;

		/* Vector con las nuevas almas creadas */
		std::vector<CEntity*> _newSouls;

		/* Crea las almas. Devuelve true cuando se acabó de crear todas las almas */
		bool createSouls();

		/* Envía a las almas a su misión. Devuelve true cuando se acabó de enviar todas las almas */
		bool sendSouls();
	};

}

#endif