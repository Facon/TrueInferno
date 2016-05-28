#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic/Entity/Message.h"
#include "Logic/Entity/Components/SoulBurner.h"

namespace AI
{
	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(Logic::CMap *map, const Logic::TEntityID& target, Logic::CSoulsTrialManager::SoulsCategory category, bool started = false) :
			CSoulTask(map, target, category), _started(started) {};

		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_map, _target, _category, _started);
		}

		bool start()
		{
			// Evitamos múltiples inicializaciones
			if (_started)
				return true;

			_started = true;

			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si existe
			if (targetEntity != nullptr)
			{
				// Incrementamos el número de almas a quemar asignadas al horno
				CSoulBurner *soulBurner = targetEntity->getComponent<CSoulBurner>();

				if (soulBurner != nullptr)
					soulBurner->increaseAssignedSoulsToBurn();
			}
			// Si no, la solución más sencilla es descartar el alma
			// TODO Quedaría mejor si le asignamos otro horno
			else
			{
				std::cout << "Soul's target for BurnSoulTask has disappeared" << std::endl;
			}

			return true;
		}

		bool execute()
		{
			CSoulTask::execute();

			// Chequeamos que el objetivo siga existiendo
			Logic::CEntity* targetEntity = _map->getEntityByID(_target);

			// Si existe
			if (targetEntity != nullptr)
			{
				// Notificamos al objetivo
				SoulBurnMessage m(1, _category);
				return m.Dispatch(*targetEntity);
			}
			// Si no
			else
			{
				std::cout << "Soul's target for BurnSoulTask has disappeared" << std::endl;
				return true;
			}
		}

		void setStarted(bool started)
		{
			_started = started;
		}

	protected:
		// Indica si el método start() ha sido ya ejecutado previamente
		bool _started = false;

	}; // class CBurnTask

} // namespace AI

#endif