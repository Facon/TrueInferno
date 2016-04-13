#include "LAReserveFromProviders.h"

#include "Logic\Entity\Message.h"
#include "AI\SMResourceDemanderData.h"
#include <algorithm>

namespace AI {
	RTTI_IMPL(CLAReserveFromProviders, CLatentAction);
	
	bool CLAReserveFromProviders::HandleMessage(const ResourceMessage& msg){
		// Esperamos únicamente confirmaciones de reserva
		if (msg._type != MessageType::RESOURCES_RESERVED)
			return false;

		// Probablemente hemos hecho algo mal si el tipo de recurso reservado no coincide con el que queríamos reservar
		assert((msg._resourceType == _smData.getResourceType()) && "Resource types reserved and expected do not match");

		// Reducimos la cantidad restante a reservar por lo reservado en este proveedor
		_remainingToReserve -= msg._quantity;

		// Actualizamos la reserva final realizada en el vector de proveedores
		_smData.getProviderMessages()[_nextProviderToReserve - 1]._quantity = msg._quantity;

		// Reactivamos la LA para seguir reservando de otros proveedores
		resume();

		return true;
	}

	/** Objeto para ordenar mensajes de recursos por distancia de un punto fijo al emisor del mensaje.
	Ordena de menor a mayor distancia */
	struct st_sorterByDistance{
		Vector3 pos;
		CMap *map;

		bool operator() (ResourceMessage i, ResourceMessage j) {
			Logic::CEntity* iEntity = map->getEntityByID(i._caller);
			if (iEntity == nullptr)
				return false; // i es null ==> Queremos j,i ==> Devolvemos false

			Logic::CEntity* jEntity = map->getEntityByID(j._caller);
			if (jEntity == nullptr)
				return true; // j es null ==> Queremos i,j ==> Devolvemos true

			// True si desde i la distancia es menor
			return (pos.squaredDistance(iEntity->getPosition()) < pos.squaredDistance(jEntity->getPosition()));
		}

	} sorterByDistance;

	CLatentAction::LAStatus CLAReserveFromProviders::OnStart() {
		// Obtenemos los mensajes de los proveedores
		std::vector<ResourceMessage> providerMessages = _smData.getProviderMessages();

		// Si no había ninguno no hay nada que hacer
		if (providerMessages.empty())
			return LAStatus::FAIL;

		// Ordenamos los mensajes por distancia a nuestra posición
		sorterByDistance.pos = _entity->getPosition();
		sorterByDistance.map = _entity->getMap();
		std::sort(providerMessages.begin(), providerMessages.end(), sorterByDistance);

		// Inicializamos la cantidad que nos queda por reservar
		_remainingToReserve = (int)_smData.getResourceQuantity();

		// Inicializamos el índice del siguiente proveedor a reservar
		_nextProviderToReserve = 0;

		return LAStatus::RUNNING;
	}

	CLatentAction::LAStatus CLAReserveFromProviders::OnRun(unsigned int msecs) {
		// Obtenemos los mensajes ya ordenados de los proveedores
		std::vector<ResourceMessage> providerMessages = _smData.getProviderMessages();

		// Fallamos si no hay ningún proveedor
		if (providerMessages.empty())
			return LAStatus::FAIL;

		// Si no queda nada por reservar: Éxito total!
		if (_remainingToReserve <= 0){
			// Registramos en el vector de proveedores que no necesitamos nada más del resto de proveedores
			for (int i = _nextProviderToReserve; i < (int)providerMessages.size(); ++i){
				providerMessages[i]._quantity = 0;
			}

			return LAStatus::SUCCESS;
		}

		// Enviaremos uno a uno los mensajes de reserva
		while (_nextProviderToReserve < (int)providerMessages.size()){
			// Leemos el mensaje que nos envió
			ResourceMessage providerMessage = providerMessages[_nextProviderToReserve];
			
			// Preparamos la siguiente iteración
			++_nextProviderToReserve;

			// Accedemos a la entidad del proveedor
			CEntity* provider = _entity->getMap()->getEntityByID(providerMessage._caller);

			// Si es nula la ignoramos
			if (provider == nullptr){
				std::cout << "Provider entity no longer exists" << std::endl;

				// Y anotamos que no reservamos nada de ella
				providerMessage._quantity = 0;

				continue;
			}
			// Solicitaremos reservar hasta cubrir lo que nos queda o lo disponible en el proveedor
			int quantityToReserve = std::min(_remainingToReserve, providerMessage._available);

			// Preparamos el mensaje para hacer la reserva
			ResourceMessage reserveMessge;
			reserveMessge.assembleResourcesReserve(providerMessage._resourceType, quantityToReserve, _entity->getEntityID());

			// Enviamos el mensaje con la reserva al proveedor. Si no recibimos respuesta reintentaremos al siguiente tick
			if (!reserveMessge.Dispatch(*provider))
				return LAStatus::RUNNING;

			// En este punto la reserva se ha aceptado pero no sabemos cuánto se ha reservado finalmente. Nos suspendemos para esperar el mensaje de confirmación
			return LAStatus::SUSPENDED;
		}

		// Llegados a este punto no hay más proveedores a los que preguntar. Hemos terminado
		// TODO Podría interesar saber si se ha podido reservar todo lo que pedimos o no
		return LAStatus::SUCCESS;
	}

}