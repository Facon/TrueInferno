#include "LASendSoul.h"

#include "Logic\Maps\EntityFactory.h"
#include "Logic\Maps\Map.h"
#include "Logic\Server.h"

#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/WorkBuilding.h"

#include "Graphics/Entity.h"

namespace AI
{
	RTTI_IMPL(CLASendSoul, CLatentAction);
	
	bool CLASendSoul::HandleMessage(const SoulSenderMessage& msg)
	{
		// Rechazamos lo que no sean peticiones
		if (msg._type != MessageType::SOUL_SENDER_REQUEST)
			return false;

		// Guardamos la información del mensaje
		_soulTasks.push(std::pair<AI::CSoulTask*, int>(msg._task, msg._numSouls));

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLASendSoul::OnStart()
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

		_numSoulsCreated = 0;

		// Suspendemos la LA hasta que llegue el mensaje con la petición
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLASendSoul::OnRun(unsigned int msecs)
	{
		// Acumulamos el tiempo transcurrido desde el último tick
		_timeSinceLastSoulSent += msecs;

		// Verificamos que tenemos alguna tarea o alma pendiente
		if (_soulTasks.empty() && _newSouls.empty())
			return LAStatus::FAIL;

		// Verificamos que se puede enviar un nuevo alma...
		if (_timeSinceLastSoulSent < _timeBetweenSouls)
			return LAStatus::RUNNING;

		// ...y reseteamos el tiempo acumulado
		_timeSinceLastSoulSent = 0;

		// Si no se pudieron crear todas las almas esperamos al siguiente tick
		if (!createSouls())
			return LAStatus::RUNNING;

		// Si no se pudieron enviar todas las almas esperamos al siguiente tick
		if (!sendSouls())
			return LAStatus::RUNNING;

		return LAStatus::SUCCESS;
	}

	bool CLASendSoul::createSouls()
	{
		// Creamos todas las almas en espera
		while (!_soulTasks.empty())
		{
			std::pair<AI::CSoulTask*, int> _soulTask = _soulTasks.front();

			// Creamos almas hasta completar las pedidas para la tarea en proceso
			for (int i = _numSoulsCreated; i < _soulTask.second; ++i)
			{
				// Creamos el alma...
				CMap* map = Logic::CServer::getSingletonPtr()->getMap();
				CEntity* newSoul = CEntityFactory::getSingletonPtr()->createEntity("Soul", map);

				if (!newSoul)
				{
					assert(false && "Can't create new soul");
					return false;
				}

				// ...y la ubicamos el  en nuestra propia posición
				Vector3 newPosition = _entity->getPosition();
				newPosition.y = SOUL_ON_TILE_HEIGHT;

				PositionMessage m(newPosition);
				if (!m.Dispatch(*newSoul))
				{
					assert(false && "Can't set soul on initial position");
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(newSoul);
					return false;
				}

				_newSouls.push(std::pair<CEntity*, AI::CSoulTask*>(newSoul, _soulTask.first->clone()));
			}

			delete _soulTask.first;
			_soulTask.first = nullptr;
			_soulTasks.pop();
		}

		return true;
	}

	bool CLASendSoul::sendSouls()
	{
		// Envíamos solamente un alma cada vez para que salgan con el retraso
		// definido entre ellas
		if (!_newSouls.empty())
		{
			std::pair<CEntity*, AI::CSoulTask*> _newSoul = _newSouls.front();

			// Asignamos la tarea al alma
			SoulMessage m(_newSoul.second);
			if (!m.Dispatch(*_newSoul.first))
				return false;

			_newSouls.pop();
			return false;
		}

		return true;
	}
}