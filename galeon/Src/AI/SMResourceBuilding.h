#ifndef SM_RESOURCE_BUILDING_H_
#define SM_RESOURCE_BUILDING_H_

#include "StateMachine.h"
#include "Logic\Entity\Message.h"
#include "AI\Server.h"
#include "AI\LAAttendResourceBuildingRequest.h"
#include "AI\SMResourceBuildingData.h"
#include "Map\MapEntity.h"

namespace AI {
	/** FSM con la lógica de un edificio de recursos para procesar operaciones de consulta y modificación de sus recursos
	Los recursos que guarda el edificio se almacenan intermante en el objeto de clase CSMResourceBuildingData.
		
	Atiende las siguientes variedades de ResourceMessage:
	- MessageType::RESOURCES_ASK: Devuelve asíncronamente un ResourceMessage de tipo MessageType::RESOURCES_INFO en respuesta.
	- MessageType::RESOURCES_CHANGE: Solicitud de modificación de recursos almacenados. Devuelve true en el HandleMessage si los ha podido cambiar.
	- MessageType::RESOURCES_RESERVE: Solicitud de reserva de recursos disponibles para que otras solicitudes no puedan reservarlos simultáneamente. Devuelve true en el HandleMessage si los ha podido reservar.
	- MessageType::RESOURCES_FREE: Solicitud de liberación de recursos reservados previamente. Devuelve true en el HandleMessage si los ha podido liberar.
	- MessageType::RESOURCES_CLAIM: Solicitud de reclamación de los recursos reservados previamente. Devuelve true en el HandleMessage si los ha podido reclamar.
	*/
	class CSMResourceBuilding : public CStateMachine<CLatentAction, CSMResourceBuildingData> {
	public:
		CSMResourceBuilding(CEntity* entity) : CStateMachine(entity) {}

		virtual ~CSMResourceBuilding() {}

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

			// Guardamos el mapa de recursos almacenables
			if (entityInfo->hasAttribute("storedResources")){
				std::istringstream ss(entityInfo->getStringAttribute("storedResources"));
				std::string item;

				// Para cada tipo separado por coma
				while (std::getline(ss, item, ',')) {
					if (item.size()>0)
						// Inicializamos la cantidad de recursos almacenados de ese tipo
						_data.registerStoredResourceType(ResourcesManager::parseResourceType(item));
				}
			}
			else{
				assert(false && "No stored resources have been defined");
			}

			// Guardamos el set con los recursos que este componente provee
			if (entityInfo->hasAttribute("providedResources")){
				std::istringstream ss(entityInfo->getStringAttribute("providedResources"));
				std::string item;

				// Para cada tipo separado por coma
				while (std::getline(ss, item, ',')) {
					// Almacenamos el tipo
					_data.getProvidedResources().emplace(ResourcesManager::parseResourceType(item));
				}
			}

			// Guardamos el máximo almacenable de cada tipo de recurso
			if (entityInfo->hasAttribute("maxResources")){
				_data.setMaxResources(entityInfo->getIntAttribute("maxResources"));
			}
			else{
				std::cout << "No resource limit has been defined" << std::endl;
			}

			// Bucle infinito procesando peticiones
			int process = this->addNode(new CLAAttendResourceBuildingRequest(entity, _data));
			this->addEdge(process, process, new CConditionFinished());

			this->setInitialNode(process);
			this->resetExecution();

			return true;
		}

		virtual void deactivate(){
			// Al desactivarnos limpiamos los recursos que quedaran
			_data.cleanResources();
		}

		SM_HANDLE_MESSAGE(ResourceMessage);
	};

}

#endif // SM_RESOURCE_BUILDING_H_