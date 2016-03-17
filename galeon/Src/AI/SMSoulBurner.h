#ifndef SM_SOUL_BURNER_
#define SM_SOUL_BURNER_

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
			assert(entityInfo->hasAttribute("cokePerSoul"));
			_cokePerSoul = entityInfo->getIntAttribute("cokePerSoul");

			assert(entityInfo->hasAttribute("crudePerSoul"));
			_crudePerSoul = entityInfo->getIntAttribute("crudePerSoul");

			assert(entityInfo->hasAttribute("burnPeriod"));
			_burnPeriod = 1000 * entityInfo->getIntAttribute("burnPeriod");

			assert(entityInfo->hasAttribute("maxSoulsPerCycle"));
			_maxSoulsPerCycle = entityInfo->getIntAttribute("maxSoulsPerCycle");

			// Creación de SM en base a los datos
			int gatherSouls = this->addNode(new CLAGatherSouls(_entity, _data, _burnPeriod, _maxSoulsPerCycle));
			int burnSouls = this->addNode(new CLABurnSouls(_entity, _data, _cokePerSoul, _crudePerSoul));

			this->addEdge(gatherSouls, burnSouls, new CConditionFinished());
			this->addEdge(burnSouls, gatherSouls, new CConditionFinished());

			this->setInitialNode(gatherSouls);
			this->resetExecution();

			return true;
		}

		bool HandleMessage(const NumberMessage& msg){
			bool ret = false;

			// Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId
			if (_currentNodeId != -1) {
				// Buscamos la lista de aristas que salen del nodo actual
				EdgeList::iterator it = _edges->find(_currentNodeId);
				if (it != _edges->end()) {
					PairVector* vector = (*it).second;
					// Para cada elemento del vector (arista que sale del nodo actual)
					for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
						// Procesamos en la arista (o sea, la condición)
						ret |= (edgeIt->first->HandleMessage(msg)); // Si alguna arista acepta, aceptaremos al final
					}
				}
			}

			return ret;
		}

	private:
		/** Cantidad de coke que da cada alma */
		int _cokePerSoul;

		/** Cantidad de crude que da cada alma */
		int _crudePerSoul;

		/** Periodo (ms) con que se queman cíclicamente las almas */
		int _burnPeriod;

		/** Número máximo de almas que se queman en cada ciclo */
		int _maxSoulsPerCycle;
	};
}

#endif