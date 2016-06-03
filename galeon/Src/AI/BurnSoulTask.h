#ifndef BURN_SOUL_TASK_H_
#define BURN_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic/Entity/Message.h"
#include "Logic/Entity/IconType.h"
#include "Logic/Entity/Components/SoulBurner.h"

namespace AI
{
	class CBurnTask : public CSoulTask {

	public:
		CBurnTask(Logic::CMap *map, const Logic::TEntityID& target, SoulsTrialManager::SoulsCategory category, bool soulAssigned = false) :
			CSoulTask(map, target, category), _soulAssigned(soulAssigned) {};

		virtual ~CBurnTask() {};

		virtual CSoulTask* clone(){
			return new CBurnTask(_map, _target, _category, _soulAssigned);
		}

		bool start()
		{
			CSoulTask::start();

			// Evitamos múltiples asignaciones a horno
			if (!_soulAssigned)
			{
				// Chequeamos que el objetivo siga existiendo
				Logic::CEntity* targetEntity = _map->getEntityByID(_target);

				// Si existe
				if (targetEntity != nullptr)
				{
					// Incrementamos el número de almas a quemar asignadas al horno
					CSoulBurner *soulBurner = targetEntity->getComponent<CSoulBurner>();

					if (soulBurner != nullptr)
						soulBurner->increaseAssignedSoulsToBurn();

					_soulAssigned = true;
				}
				// Si no, la solución más sencilla es descartar el alma
				// TODO Quedaría mejor si le asignamos otro horno
				else
				{
					assert(false && "Soul's target for BurnSoulTask has disappeared");
					return false;
				}
			}

			// Ponemos los iconos
			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			// Si no, dejamos que siga sin icono
			if (executor != nullptr)
			{
				// Icono de alma quemándose
				IconMessage m(MessageType::ICON_ADD, IconType::IconType::BURNING);
				bool result = m.Dispatch(*executor);
				assert(result && "Can't set burning soul icon");

				// Icono por edificio de destino
				result = addDestinationBuildingIcon();
				assert(result && "Can't set building icon");
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

		void setSoulAssigned(bool soulAssigned)
		{
			_soulAssigned = soulAssigned;
		}

	protected:
		// Indica si el alma a quemar ya ha sido asignada a un horno
		bool _soulAssigned = false;

	}; // class CBurnTask

} // namespace AI

#endif