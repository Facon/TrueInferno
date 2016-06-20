#ifndef REQUEST_TRANSPORT_SOUL_TASK_H_
#define REQUEST_TRANSPORT_SOUL_TASK_H_

#include "SoulTask.h"
#include "Logic\Entity\Message.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/Billboard.h"
#include "Logic\Entity\BuildingType.h"
#include "Logic\Entity\IconType.h"

using namespace Logic;

namespace AI{

	class CRequestTransportSoulTask : public CSoulTask {

	public:
		CRequestTransportSoulTask(CMap *map, const TEntityID& fromId, const TEntityID& toId, ResourceType resourceType, unsigned int resourceQuantity,
			Logic::SoulsTrialManager::SoulsCategory category = Logic::SoulsTrialManager::SoulsCategory::UNKNOWN) :
			CSoulTask(map, fromId, category), // El objetivo es el punto de inicio donde solicitaremos el transporte
			_toId(toId),
			_resourceType(resourceType),
			_resourceQuantity(resourceQuantity) {};

		virtual ~CRequestTransportSoulTask() {};

		virtual CSoulTask* clone(){
			return new CRequestTransportSoulTask(_map, _target, _toId, _resourceType, _resourceQuantity, _category);
		}

		bool start(){
			CSoulTask::start();

			// Obtenemos la entidad que va a ejecutar la tarea
			CEntity* executor = _map->getEntityByID(_executorId);

			// Si existe, establecemos sus iconos
			if (executor != nullptr){
				// Icono del recurso que va a ir a recoger la entidad (distinto de recurso siendo transportado)
				//IconMessage m(MessageType::ICON, Billboard::getResourceIcon(_resourceType));
				// De momento ponemos la pala que es lo más parecido a un transporte que tenemos
				IconMessage m(MessageType::ICON_ADD, IconType::IconType::MINE);
				
				const bool result = m.Dispatch(*executor);
				assert(result && "Can't set transport soul icon");

				// Icono por edificio de destino
				const bool result2 = addDestinationBuildingIcon();
				assert(result2 && "Can't set building icon");
			}

			else{
				assert(false && "There is no executor starting the task");
				// Dejamos que siga sin icono
			}

			return true;
		}

		bool execute() {
			CSoulTask::execute();

			// Chequeamos que el objetivo al que acabamos de llegar siga existiendo
			CEntity* targetEntity = _map->getEntityByID(_target);

			// Si lo está
			if (targetEntity != nullptr){
				// Notificamos al objetivo para que lleve los recursos al destino
				LogisticsMessage m;
				m.assembleProvideResources(_resourceType, _resourceQuantity, _toId);
				return m.Dispatch(*targetEntity);
			}

			// Si no
			else{
				std::cout << "Soul's target for RequestTransportSoulTask has disappeared" << std::endl;
				return true;
			}
		};

	private:
		// Entidad desde la que debe partir el alma para comenzar el transporte. Es el objetivo inicial, _target, hacia el que debe ir el alma
		//TEntityID _fromId;

		// Entidad hasta la que deberá ir finalmente el alma
		TEntityID _toId;

		// Tipo de recurso que portará el alma
		ResourceType _resourceType;

		// Cantidad de recursos que portará el alma
		unsigned int _resourceQuantity;

	}; // class CRequestTransportSoulTask

} // namespace AI

#endif // REQUEST_TRANSPORT_SOUL_TASK_H_