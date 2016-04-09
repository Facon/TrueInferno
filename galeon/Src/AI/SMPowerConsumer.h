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
			int findGenerator = this->addNode(new CLAFindGenerator(entity, _data));
			int attachToGenerator = this->addNode(new CLAAttachToGenerator(entity, _data));
			int waitDetachment = this->addNode(new CLAWaitGeneratorDetachment(entity, _data));

			// La entidad comienza en el estado de búsqueda de generador
			this->setInitialNode(findGenerator);

			// Permanece en búsqueda hasta que encuentra uno y pasa a intentar conectarse
			this->addEdge(findGenerator, attachToGenerator, new CConditionSuccess());
			this->addEdge(findGenerator, findGenerator, new CConditionFail());

			// Si la conexión va bien pasa al estado de espera, si no, vuelve a buscar otro generador
			this->addEdge(attachToGenerator, waitDetachment, new CConditionSuccess());
			this->addEdge(attachToGenerator, findGenerator, new CConditionFail());

			// En estado de espera no se hace nada hasta recibir desconexión.Si se desconecta, intenta buscar otro generador
			this->addEdge(waitDetachment, findGenerator, new CConditionSuccess());
			this->addEdge(waitDetachment, waitDetachment, new CConditionFail());

			this->resetExecution();
		}

		virtual ~CSMPowerConsumer() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){
			return true;
		}

		SM_HANDLE_MESSAGE(PowerMessage);

		virtual void deactivate(){
			// Al desactivarnos nos desconectamos del generador
			if (_data.getAttached() && _data.getPowerGenerator()!=EntityID::UNASSIGNED){
				CEntity *generator = _entity->getMap()->getEntityByID(_data.getPowerGenerator());
				if (generator != nullptr){
					PowerMessage m;
					m.assemblePowerAttachmentInfo(_entity->getEntityID(), false, 10); // TODO
					assert(m.Dispatch(*generator) && "Can't detach from PowerGenerator on deactivation");
				}
			}
		}

	};
}

#endif // SM_POWER_CONSUMER_H_