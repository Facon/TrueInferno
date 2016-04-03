#include "LAAttendResourceBuildingRequest.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceBuildingData.h"

#include "Logic\Maps\EntityFactory.h"

namespace AI {
	bool CLAAttendResourceBuildingRequest::HandleMessage(const ResourceMessage& msg) {
		// No se aceptan peticiones simult�neas
		if (_requestReceived)
			return false;

		// Si es una solicitud de modificaci�n de recursos
		if (msg._type == MessageType::RESOURCES_CHANGE){
			// Rechazamos si el recurso no lo almacenamos
			if (!_smData.isResourceTypeStored(msg._resourceType))
				return false;

			// Intentamos hacer el cambio
			return _smData.changeStoredResources(msg._resourceType, msg._quantity);
		}

		// Si es una solicitud de reserva de recursos
		else if (msg._type == MessageType::RESOURCES_RESERVE){
			// Rechazamos si el recurso no lo almacenamos
			if (!_smData.isResourceTypeStored(msg._resourceType))
				return false;
		}

		// Si es una solicitud de liberaci�n de recursos reservados
		else if (msg._type == MessageType::RESOURCES_FREE){
			// Rechazamos si el recurso no lo almacenamos
			if (!_smData.isResourceTypeStored(msg._resourceType))
				return false;

			// Intentamos hacer la liberaci�n
			return _smData.freeReservedResources(msg._resourceType, msg._quantity);
		}

		// Si es una solicitud de reclamaci�n de recursos
		else if (msg._type == MessageType::RESOURCES_CLAIM){
			// Rechazamos si el recurso no lo almacenamos
			if (!_smData.isResourceTypeStored(msg._resourceType))
				return false;

			// Intentamos hacer la reclamaci�n
			return _smData.claimReservedResources(msg._resourceType, msg._quantity);
		}

		// Si es una petici�n de informaci�n de cantidad de recurso
		else if (msg._type == MessageType::RESOURCES_ASK){
			// Rechazamos si el recurso no lo almacenamos
			if (!_smData.isResourceTypeStored(msg._resourceType))
				return false;
		}

		// Rechazamos todo lo dem�s
		else{
			return false;
		}
		
		// Registramos que ya hemos recibido una petici�n este tick
		_requestReceived = true;

		// Guardamos el mensaje recibido
		_msgReceived = msg;

		// Reactivamos la LA
		resume();

		return true;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::OnStart() {
		// Inicializamos
		_requestReceived = false;
		_msgReceived = ResourceMessage();
		_finallyReserved = 0;
		_processDone = false;

		// Suspendemos la LA hasta que llegue un mensaje de petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::OnRun(unsigned int msecs) {
		// Procesamos aqu� las peticiones que necesitan enviar mensaje de respuesta

		// Si es de cambio de recursos
		/*if (_msgReceived._type == MessageType::RESOURCES_CHANGE){
			return processResourcesChange();
		}*/

		// Si es de petici�n de informaci�n
		if (_msgReceived._type == MessageType::RESOURCES_ASK){
			return processAskResources();
		}

		// Si es de petici�n de reserva
		else if (_msgReceived._type == MessageType::RESOURCES_RESERVE){
			return processReserveResources();
		}
	
		return LAStatus::SUCCESS;
	}

	/*CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesChange(){
		int change = _msgReceived._quantity;

		// No deber�amos tener que procesar un mensaje con 0 recursos a cambiar
		if (change == 0){
			assert(false && "Discarding message with 0 resoures to change");
			return LAStatus::SUCCESS;
		}

		// Los cambiamos
		_smData.getStoredResources()[_msgReceived._resourceType] += change;

		/** No deber�an haber salido negativos ni mayores que el l�mite porque hicimos el chequeo en OnStart!
		Por seguridad y coherencia de los datos chequeamos y ajustamos el cambio realizado
		*
		int newQuantity = _smData.getStoredResources()[_msgReceived._resourceType];
		if (newQuantity < 0){
			assert(false && "Changing resources below 0");

			/** Si cantidad = 8 y cambio = -10 => nuevaCantidad = -2
			Ajustamos: cambio (-10) -= nuevaCantidad (-2) = -8
			Y nuevaCantidad = 0 *
			change -= newQuantity;
			newQuantity = 0;
		}
		else if (newQuantity > (int) _smData.getMaxResources()){
			assert(false && "Changing resources over limit");

			/** Si cantidad = 8 y cambio = 4 => nuevaCantidad = 12 (aunque el l�mite sea 10)
			Ajustamos: cambio (4) -= (nuevaCantidad (12) - l�mite (10)) = 2
			Y nuevaCantidad = l�mite = 10 *
			change -= (newQuantity - _smData.getMaxResources());
			newQuantity = _smData.getMaxResources();
		}

		// Notificamos al ResourcesManager
		// TODO Atajo temporal para obtener el ResourcesManager
		Logic::ResourcesManager *resourcesManager =
			((Application::CGameState*) Application::CGaleonApplication::getSingletonPtr()->getState())->getResourcesManager();
		resourcesManager->changeResources(_msgReceived._resourceType, change);

		// Y finalizamos
		return LAStatus::SUCCESS;
	}*/

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processAskResources(){
		// Buscamos la entidad que solicit� la informaci�n
		CEntity* recipient = _entity->getMap()->getEntityByID(_msgReceived._caller);
		if (recipient == nullptr){
			std::cout << "Caller entity no longer exists" << std::endl;
			return LAStatus::FAIL;
		}

		// Preparamos el mensaje de repuesta RESOURCES_INFO
		ResourceType resourceType = _msgReceived._resourceType;
		int stored = _smData.getStoredResources(resourceType);
		int available = _smData.getAvailableResources(resourceType);
		bool provided = _smData.getProvidedResources().count(resourceType) > 0;
		ResourceMessage m;
		m.assembleResourcesInfo(resourceType, stored, available, _smData.getMaxResources(), provided, _entity->getEntityID());
		
		// Enviamos el mensaje para que lo acepte
		if (m.Dispatch(*recipient))
			return LAStatus::SUCCESS;

		// O esperamos al siguiente tick
		// TODO HAY que poner l�mite a los reintentos. La entidad puede estar "muerta" aunque el puntero no sea nulo
		else
			return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processReserveResources(){
		// Buscamos la entidad que solicit� la informaci�n
		CEntity* recipient = _entity->getMap()->getEntityByID(_msgReceived._caller);
		if (recipient == nullptr){
			std::cout << "Caller entity no longer exists" << std::endl;

			if (_processDone)
				_smData.freeReservedResources(_msgReceived._resourceType, _finallyReserved);

			return LAStatus::FAIL;
		}

		// Se intenta reservar hasta lo solicitado si no se hab�a hecho ya la operaci�n
		if (!_processDone){
			if (_smData.reserveResources(_msgReceived._resourceType, _msgReceived._quantity, true, _finallyReserved)){
				_processDone = true;
			}

			// Dado que se permiten reservas parciales, un retorno falso probablemente implique error
			else{
				assert("Error reserving resources" && false);
				_finallyReserved = 0;
				_processDone = true;
			}
		}

		// Preparamos el mensaje de repuesta RESOURCES_RESERVED con lo finalmente reservado
		ResourceMessage m;
		m.assembleResourcesReserved(_msgReceived._resourceType, _finallyReserved);

		// Enviamos el mensaje para que lo acepte
		if (m.Dispatch(*recipient))
			return LAStatus::SUCCESS;

		// O esperamos al siguiente tick
		// TODO HAY que poner l�mite a los reintentos. La entidad puede estar "muerta" aunque el puntero no sea nulo
		else
			return LAStatus::RUNNING;
	}

}