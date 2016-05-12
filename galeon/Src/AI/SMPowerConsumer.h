#ifndef SM_POWER_CONSUMER_H_
#define SM_POWER_CONSUMER_H_

#include "Logic\Entity\Message.h"

#include "AI\StateMachine.h"
#include "AI\Server.h"
#include "AI\LAFindGenerator.h"
#include "AI\LAAttachToGenerator.h"
#include "AI\LAWaitGeneratorDetachment.h"
#include "AI\SMPowerConsumerData.h"

namespace AI {
	/**
	Máquina de estados para los consumidores de energía PowerConsumer.
	- La entidad comienza en el estado de búsqueda de generador
	- Permanece en búsqueda hasta que encuentra uno y pasa a intentar conectarse
	- Si la conexión va bien pasa al estado de espera, si no, vuelve a buscar otro generador
	- En estado de espera no se hace nada hasta recibir desconexión. Si se desconecta, intenta buscar otro generador
	*/
	class CSMPowerConsumer : public CStateMachine<CLatentAction, CSMPowerConsumerData> {
	public:
		CSMPowerConsumer(CEntity* entity) : CStateMachine(entity) {
			_name = "PowerConsumer";
			_debug = false;
		}

		virtual ~CSMPowerConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			// Lectura de datos
			assert(entityInfo->hasAttribute("consumptionUnits"));
			_consumptionUnits = entityInfo->getIntAttribute("consumptionUnits");

			assert(entityInfo->hasAttribute("consumptionPeriod"));
			_consumptionPeriod = 1000 * entityInfo->getIntAttribute("consumptionPeriod");

			// Creación de SM en base a los datos
			int findGenerator = this->addNode(new CLAFindGenerator(entity, _data));
			int attachToGenerator = this->addNode(new CLAAttachToGenerator(entity, _data, _consumptionUnits, _consumptionPeriod));
			int waitDetachment = this->addNode(new CLAWaitGeneratorDetachment(entity, _data));

			// La entidad comienza en el estado de búsqueda de generador
			this->setInitialNode(findGenerator);

			// Permanece en búsqueda hasta que encuentra uno y pasa a intentar conectarse
			this->addEdge(findGenerator, attachToGenerator, new CConditionSuccess());
			this->addEdge(findGenerator, findGenerator, new CConditionFail());

			// Si la conexión va bien pasa al estado de espera, si no, vuelve a buscar otro generador
			this->addEdge(attachToGenerator, waitDetachment, new CConditionSuccess());
			this->addEdge(attachToGenerator, findGenerator, new CConditionFail());

			// En estado de espera no se hace nada hasta recibir desconexión. Si se desconecta, intenta buscar otro generador
			this->addEdge(waitDetachment, findGenerator, new CConditionSuccess());
			this->addEdge(waitDetachment, waitDetachment, new CConditionFail());

			this->resetExecution();

			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);

		virtual void deactivate(){
			// Al desactivarnos nos desconectamos del generador
			if (_data.getAttached() && _data.getPowerGenerator()!=EntityID::UNASSIGNED){
				CEntity *generator = _entity->getMap()->getEntityByID(_data.getPowerGenerator());
				if (generator != nullptr){
					PowerMessage m;
					m.assemblePowerAttachmentInfo(_entity->getEntityID(), false, _consumptionUnits, _consumptionPeriod);
					assert(m.Dispatch(*generator) && "Can't detach from PowerGenerator on deactivation");
				}
			}
		}

	private:
		// Unidades de recurso de energía consumido por periodo de tiempo
		int _consumptionUnits;

		// Periodo (ms) con que se consumen recursos de energía
		int _consumptionPeriod;
	};
}

#endif // SM_POWER_CONSUMER_H_