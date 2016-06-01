#include "SMResourceBuildingData.h"

#include <iostream>
#include <cassert>

#include "Logic\Entity\Message.h"
#include "Logic\ResourcesManager.h"
#include "Logic\Entity\Components\Billboard.h"

using namespace Logic;

namespace AI {
	/** Registra el tipo de recurso dado para ser almacenado */
	void CSMResourceBuildingData::registerStoredResourceType(ResourceType type, int initialResources, int maxResources) {
		// Primero registramos los tipos en los arrays
		_storedResources[type] = 0;
		_maxResources[type] = 0;
		_reservedResources[type] = 0;

		// Después inicializamos el valor máximo y el inicial
		changeMaxResources(type, maxResources);
		changeStoredResources(type, initialResources);
	}

	/** Devuelve true o false según si el tipo de recurso es almacenable en esta entidad o no */
	bool CSMResourceBuildingData::isResourceTypeStored(ResourceType type) const {
		return (_storedResources.count(type) > 0);
	}

	/** Devuelve la cantidad de recursos almacenados del tipo dado */
	int CSMResourceBuildingData::getStoredResources(ResourceType type) const {
		if (!isResourceTypeStored(type)){
			return 0;
		}

		return _storedResources.at(type);
	}

	/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
	int CSMResourceBuildingData::getAvailableResources(ResourceType type) const {
		if (!isResourceTypeStored(type)){
			return 0;
		}

		return _storedResources.at(type) - _reservedResources.at(type);
	}

	unsigned int CSMResourceBuildingData::getMaxResources(ResourceType type) const{
		if (!isResourceTypeStored(type)){
			return 0;
		}

		return _maxResources.at(type);
	}

	/** Modifica los recursos del tipo dado según la cantidad positiva o negativa indicada.
	Devuelve true o false según si la operación se realizó correctamente o no */
	bool CSMResourceBuildingData::changeStoredResources(ResourceType type, int quantity){
		if (!isResourceTypeStored(type)){
			return false;
		}

		if (quantity == 0)
			return true;

		int newStored = _storedResources[type] + quantity;

		// Controlamos que el nuevo valor no sobrepase los límites
		if (newStored < 0){
			//std::cout << "Discarded changing " << quantity << " resources because there are only stored " << _storedResources[type] << std::endl;
			
			// Mostramos icono de recurso necesario porque no pudimos satisfacer el cambio
			IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(type));
			bool result = m.Dispatch(*_owner);
			assert(result && "Can't add resource icon");

			return false;
		}

		else if (newStored > (int)_maxResources[type]){
			//std::cout << "Discarded changing " << quantity << " resources because there are already stored " << _storedResources[type] << " (limit = " << _maxResources[type] << ")" << std::endl;
			return false;
		}

		// Si hemos pasado de la nada a tener algo
		if ((_storedResources[type] == 0) && (newStored > 0)){
			// Quitamos icono de recurso necesario (si estaba puesto)
			IconMessage m(MessageType::ICON_DELETE, Billboard::getResourceIcon(type));
			bool result = m.Dispatch(*_owner);
			assert(result && "Can't delete resource icon");
		}

		// Registramos el nuevo valor con el cambio aplicado
		_storedResources[type] = newStored;

		// Notificamos al ResourcesManager
		Logic::ResourcesManager* resourcesManager = ResourcesManager::getSingletonPtr();
		resourcesManager->changeDisplayedResources(type, quantity);

		return true;
	}

	bool CSMResourceBuildingData::changeMaxResources(ResourceType type, int quantity) {
		if (!isResourceTypeStored(type)){
			return false;
		}

		if (quantity == 0)
			return true;

		int newMax = getMaxResources(type) + quantity;

		// Controlamos que el nuevo valor no sobrepase los límites
		if (newMax < 0){
			std::cout << "Discarded changing " << quantity << " max resources because current maximum is " << getMaxResources(type) << std::endl;
			return false;
		}

		// Registramos el nuevo valor con el cambio aplicado
		_maxResources[type] = newMax;

		// Notificamos al ResourcesManager
		Logic::ResourcesManager* resourcesManager = ResourcesManager::getSingletonPtr();
		resourcesManager->changeDisplayedMaxResources(type, quantity);

		return true;
	}

	/** Reserva los recursos del tipo dado según la cantidad positiva indicada. 
	Con el flag allowPartial a true se permiten reservas parciales, esto es, se reserva todo lo que haya disponible aunque no llegue a lo solicitado.
	En finallyReserved se almacena la cantidad finalmente reservada.
	Devuelve true o false según si la operación se realizó correctamente o no */
	bool CSMResourceBuildingData::reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved){
		if (!isResourceTypeStored(type)){
			return false;
		}

		if (quantity <= 0){
			assert(false && "Reserved quantity must be > 0");
			return false;
		}

		int newReserved = _reservedResources[type] + quantity;

		// Si se sobrepasa la cantidad disponible para reservar
		if (newReserved > getAvailableResources(type)){
			// Fallamos si no se deseaba reserva parcial
			if (!allowPartial){
				std::cout << "Discarded reserving " << quantity << " resources because there are only available " << getAvailableResources(type) << std::endl;
				finallyReserved = 0;

				// Mostramos icono de recurso necesario porque no pudimos satisfacer la reserva
				IconMessage m(MessageType::ICON_ADD, Billboard::getResourceIcon(type));
				bool result = m.Dispatch(*_owner);
				assert(result && "Can't add resource icon");

				return false;
			}

			// Si se permiten, reservamos hasta donde sea posible
			else{
				finallyReserved = getAvailableResources(type);
				_reservedResources[type] = _reservedResources[type] + finallyReserved;
				return true;
			}
		}

		_reservedResources[type] = newReserved;
		finallyReserved = quantity;

		return true;
	}

	/** Libera los recursos reservados del tipo dado según la cantidad positiva indicada.
	Devuelve true o false según si la operación se realizó correctamente o no */
	bool CSMResourceBuildingData::freeReservedResources(ResourceType type, int quantity, bool afterClaim){
		if (!isResourceTypeStored(type)){
			return false;
		}

		if (quantity <= 0){
			assert(false && "Freed quantity must be > 0");
			return false;
		}

		int newReserved = _reservedResources[type] - quantity;

		// Controlamos que no se libere más de lo que estaba reservado
		if (newReserved < 0){
			assert(false && "Can't free resources that weren't reserved");
			return false;
		}

		// Quitamos icono de recurso necesario (si estaba puesto) porque nos han liberado algo
		// Únicamente lo hacemos si no viene por un claim. En ese caso previamente se ha tomado de las reservas lo reclamado por lo que no hay liberación real
		if (!afterClaim){
			IconMessage m(MessageType::ICON_DELETE, Billboard::getResourceIcon(type));
			bool result = m.Dispatch(*_owner);
			assert(result && "Can't delete resource icon");
		}

		_reservedResources[type] = newReserved;
		return true;
	}

	/** Reclama los recursos previamente reservados del tipo dado en la cantidad indicada
	Devuelve true o false según si la operación se realizó correctamente o no */
	bool CSMResourceBuildingData::claimReservedResources(ResourceType type, int quantity){
		if (!changeStoredResources(type, -quantity))
			return false;
			
		return freeReservedResources(type, quantity, true);
	}

	std::unordered_set<ResourceType>& CSMResourceBuildingData::getProvidedResources() {
		return _providedResources;
	}

	/*void CSMResourceBuildingData::registerStoredResourceType(std::unordered_set<ResourceType>& providedResources) {
		providedResources = _providedResources;
	}*/

	void CSMResourceBuildingData::deactivate() {
		/** Limpia los recursos almacenados y máximos */
		for (auto it = _storedResources.cbegin(); it != _storedResources.cend(); ++it){
			ResourceType type = it->first;
			int quantity = it->second;

			// Eliminamos los recursos que había almacenados
			if (quantity > 0)
				changeStoredResources(type, -quantity);

			// Eliminamos la capacidad de almacenamiento
			changeMaxResources(type, -(int)getMaxResources(type));
		}
	}
}
