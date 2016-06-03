#ifndef SM_SOUL_H_BURNER_
#define SM_SOUL_H_BURNER_

#include "Logic\Entity\Message.h"
#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\SMSoulBurnerData.h"
#include "AI\LAGatherSouls.h"
#include "AI\LABurnSouls.h"
#include "AI\LatentAction.h"
#include "AI\Condition.h"
#include "Map\MapEntity.h"

namespace AI {
	/**
	Esta FSM controla la lógica fundamental del horno.

	El horno acumula almas a medida que llegan. Cíclicamente se queman y producen recursos.
	*/ 
	class CSMSoulBurner : public CStateMachine<CLatentAction, CSMSoulBurnerData> {
	public:
		CSMSoulBurner(CEntity* entity) : CStateMachine<CLatentAction, CSMSoulBurnerData>(entity) {}

		virtual ~CSMSoulBurner() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			assert(entityInfo->hasAttribute("burnPeriod"));
			_burnPeriod = 1000 * entityInfo->getIntAttribute("burnPeriod");

			assert(entityInfo->hasAttribute("maxSoulsPerCycle"));
			_maxSoulsPerCycle = entityInfo->getIntAttribute("maxSoulsPerCycle");

			// Creación de SM en base a los datos
			int gatherSouls = this->addNode(new CLAGatherSouls(_entity, _data, _burnPeriod, _maxSoulsPerCycle));
			int burnSouls = this->addNode(new CLABurnSouls(_entity, _data));

			this->addEdge(gatherSouls, burnSouls, new CConditionFinished());
			this->addEdge(burnSouls, gatherSouls, new CConditionFinished());

			this->setInitialNode(gatherSouls);
			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(SoulBurnMessage);

		/** Devuelve el número virtual de almas a ser quemadas en esta entidad */
		int getVirtualSoulsToBurn()
		{
			return _data.getVirtualNumSoulsToBurn();
		}

		/** Devuelve el número actual de almas a ser quemadas en esta entidad */
		int getCurrentSoulsToBurn()
		{
			return _data.getNumSoulsToBurn();
		}

		/** Devuelve el número máximo de almas que pueden quemarse en cada ciclo */
		int getMaxSoulsToBurn()
		{
			return _maxSoulsPerCycle;
		}

		/** Incrementa el número de almas a quemar asignadas pero aún en camino. */
		void increaseAssignedSoulsToBurn()
		{
			_data.increaseAssignedSoulsToBurn();
		}

	private:

		/** Periodo (ms) con que se queman cíclicamente las almas */
		int _burnPeriod;

		/** Número máximo de almas que se queman en cada ciclo */
		int _maxSoulsPerCycle;
	};
}

#endif