#ifndef LA_SEND_SOUL_
#define LA_SEND_SOUL_

#include "LatentAction.h"
#include "Server.h"
#include "SoulTask.h"
#include "WorkSoulTask.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/EntityFactory.h"

#include <queue>
#include <utility>

namespace AI
{
	class CLASendSoul : public CLatentAction {
		RTTI_DECL;

	public:
		CLASendSoul(CEntity* entity) : CLatentAction(entity), _soulTasks(), _newSouls()
		{}

		virtual ~CLASendSoul()
		{
			// Limpiamos el vector de tareas
			while (!_soulTasks.empty())
			{
				delete _soulTasks.front().first;
				_soulTasks.front().first = nullptr;
				_soulTasks.pop();
			}

			// Limpiamos el vector de almas
			while (!_newSouls.empty())
			{
				delete _newSouls.front().second;
				_newSouls.front().second = nullptr;
				_newSouls.pop();
			}
		}

		virtual bool HandleMessage(const SoulSenderMessage& msg);

	protected:
		LAStatus OnStart();

		LAStatus OnRun(unsigned int msecs);

	private:
		const float SOUL_ON_TILE_HEIGHT = 2.0;

		/**
		Cola para almacenar la informaci�n de los mensajes procesados. Contiene las
		tareas a asignar a las almas junto con el n�mero de almas que se enviar�n
		por cada tarea.
		*/
		std::queue<std::pair<AI::CSoulTask*, int>> _soulTasks;

		/**
		Cola con las nuevas almas creadas junto con la tarea a asignarle a cada una.
		*/
		std::queue<std::pair<CEntity*, AI::CSoulTask*>> _newSouls;

		/* N�mero de almas ya creadas para la tarea en proceso */
		int _numSoulsCreated;

		/* Tiempo transcurrido desde el �ltimo alma enviada */
		unsigned int _timeSinceLastSoulSent = 0;

		/* Tiempo a esperar entre cada alma enviada */
		unsigned int _timeBetweenSouls = 200;

		/* Crea las almas. Devuelve true cuando se acab� de crear todas las almas */
		bool createSouls();

		/* Env�a a las almas a su misi�n. Devuelve true cuando se acab� de enviar todas las almas */
		bool sendSouls();
	};

}

#endif