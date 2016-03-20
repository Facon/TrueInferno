#include "LAAttendResourceBuildingRequest.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceBuildingData.h"

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"

#include "Logic\Maps\EntityFactory.h"

namespace AI {
	bool CLAAttendResourceBuildingRequest::HandleMessage(const ResourceMessage& msg) {
		// No se aceptan peticiones simult�neas
		if (_requestReceived)
			return false;

		// Si es una solicitud de cambio de recursos
		if (msg._type == MessageType::RESOURCES_CHANGE){
			// Rechazamos si el recurso no lo almacenamos
			if (_smData.getStoredResources().count(msg._resourceType) == 0)
				return false;

			// Verificamos que el cambio de recursos no desborde los l�mites
			int newQuantity = _smData.getStoredResources()[msg._resourceType] + msg._change;
			if (newQuantity < 0){
				// Rechazamos para no quedarnos en negativo
				std::cout << "Discarded changing (" << msg._change << ") resources below 0: " << newQuantity << std::endl;
				return false;
			}
			else if (newQuantity > (int)_smData.getMaxResources()){
				// Rechazamos para no sobrepasar el m�ximo
				std::cout << "Discarded changing (" << msg._change << ") resources above limit: (" << _smData.getMaxResources() << "): " << newQuantity << std::endl;
				return false;
			}
		}

		// Si es una petici�n de informaci�n de cantidad de recurso
		else if (msg._type == MessageType::RESOURCES_ASK){
			// Rechazamos si el recurso no lo almacenamos
			if (_smData.getStoredResources().count(msg._resourceType) == 0)
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

		// Suspendemos la LA hasta que llegue un mensaje de petici�n
		return LAStatus::SUSPENDED;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::OnRun(unsigned int msecs) {
		// Procesamos la petici�n

		// Si es de cambio de recursos
		if (_msgReceived._type == MessageType::RESOURCES_CHANGE){
			return processResourcesChange();
		}

		// Si es de petici�n de informaci�n
		else if (_msgReceived._type == MessageType::RESOURCES_ASK){
			return processResourcesInfo();
		}

		// En otro caso no deber�amos haber llegado aqu� porque el mensaje no deber�a haberse aceptado
		else{
			assert(false && "Can't process this message type");
			return LAStatus::FAIL;
		}
		
		return LAStatus::SUCCESS;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesChange(){
		int change = _msgReceived._change;

		// No deber�amos tener que procesar un mensaje con 0 recursos a cambiar
		if (change == 0){
			assert(false && "Discarding message with 0 resoures to change");
			return LAStatus::SUCCESS;
		}

		// Los cambiamos
		_smData.getStoredResources()[_msgReceived._resourceType] += change;

		/** No deber�an haber salido negativos ni mayores que el l�mite porque hicimos el chequeo en OnStart!
		Por seguridad y coherencia de los datos chequeamos y ajustamos el cambio realizado
		*/
		int newQuantity = _smData.getStoredResources()[_msgReceived._resourceType];
		if (newQuantity < 0){
			assert(false && "Changing resources below 0");

			/** Si cantidad = 8 y cambio = -10 => nuevaCantidad = -2
			Ajustamos: cambio (-10) -= nuevaCantidad (-2) = -8
			Y nuevaCantidad = 0 */
			change -= newQuantity;
			newQuantity = 0;
		}
		else if (newQuantity > (int) _smData.getMaxResources()){
			assert(false && "Changing resources over limit");

			/** Si cantidad = 8 y cambio = 4 => nuevaCantidad = 12 (aunque el l�mite sea 10)
			Ajustamos: cambio (4) -= (nuevaCantidad (12) - l�mite (10)) = 2
			Y nuevaCantidad = l�mite = 10 */
			change -= (newQuantity - _smData.getMaxResources());
			newQuantity = _smData.getMaxResources();
		}

		// Notificamos al ResourcesManager
		// TODO Atajo temporal para obtener el ResourcesManager
		Logic::ResourcesManager *resourcesManager =
			((Application::CGameState*) Application::CGaleonApplication::getSingletonPtr()->getState())->getResourcesManager();
		resourcesManager->increaseResources(_msgReceived._resourceType, change);

		// Y finalizamos
		return LAStatus::SUCCESS;
	}

	CLatentAction::LAStatus CLAAttendResourceBuildingRequest::processResourcesInfo(){
		// Preparamos el mensaje de repuesta RESOURCES_INFO
		ResourceType resourceType = _msgReceived._resourceType;
		int resourceQuantity = _smData.getStoredResources()[resourceType];
		bool provides = _smData.getProvidedResources().count(resourceType) > 0;
		ResourceMessage m;
		m.assembleResourcesInfo(resourceType, resourceQuantity, _smData.getMaxResources(), provides, _entity->getEntityID());
		
		// Buscamos la entidad que solicit� la informaci�n
		CEntity* recipient = _entity->getMap()->getEntityByID(_msgReceived._caller);

		// Le enviamos el mensaje a ver si lo acepta
		if (m.Dispatch(*recipient))
			return LAStatus::SUCCESS;

		// O esperamos al siguiente tick
		// TODO Hay que poner un l�mite a los reintentos. La entidad podr�a no existir
		else
			return LAStatus::RUNNING;
	}
	
}