#ifndef SM_RESOURCE_BUILDING_DATA_
#define SM_RESOURCE_BUILDING_DATA_

#include "Application/GaleonApplication.h"
#include "Application/GameState.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream> 

namespace AI {
	class CSMResourceBuildingData {

	public:
		CSMResourceBuildingData() : _maxResources(0) {
			_storedResources.clear();
			_reservedResources.clear();
			_providedResources.clear();
		}

		virtual ~CSMResourceBuildingData() {
			_storedResources.clear();
			_reservedResources.clear();
			_providedResources.clear();
		}

		/** Registra el tipo de recurso dado para ser almacenado */
		void registerStoredResourceType(ResourceType type) {
			// Inicializamos los recursos almacenados de ese tipo a 0
			_storedResources[type] = 0;

			// Ídem para los recursos reservables
			_reservedResources[type] = 0;
		}

		/** Devuelve true o false según si el tipo de recurso es almacenable en esta entidad o no */
		bool isResourceTypeStored(ResourceType type){
			return (_storedResources.count(type) > 0);
		}

		/** Devuelve la cantidad de recursos almacenados del tipo dado */
		int getStoredResources(ResourceType type) {
			if (!isResourceTypeStored(type)){
				assert(false && "Resource not stored. Check wether it's stored before doing anything!");
				return 0;
			}

			return _storedResources[type];
		}

		/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
		int getAvailableResources(ResourceType type) {
			if (!isResourceTypeStored(type)){
				assert(false && "Resource not stored. Check wether it's stored before doing anything!");
				return 0;
			}

			return _storedResources[type] - _reservedResources[type];
		}

		/** Modifica los recursos del tipo dado según la cantidad positiva o negativa indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool changeStoredResources(ResourceType type, int quantity){
			if (!isResourceTypeStored(type)){
				assert(false && "Resource not stored. Check wether it's stored before doing anything!");
				return 0;
			}

			int aux = _storedResources[type] + quantity;

			// Controlamos que el nuevo valor no sobrepase los límites
			if (aux < 0){
				std::cout << "Discarded changing " << quantity << " resources because there are only stored " << _storedResources[type] << std::endl;
				return false;
			}

			else if (aux > (int)_maxResources){
				std::cout << "Discarded changing " << quantity << " resources because there are already stored " << _storedResources[type] << " (limit = " << _maxResources << ")" << std::endl;
				return false;
			}

			// Registramos el nuevo valor con el cambio aplicado
			_storedResources[type] = aux;

			// Notificamos al ResourcesManager
			// TODO Atajo temporal para obtener el ResourcesManager
			Logic::ResourcesManager *resourcesManager =
				((Application::CGameState*) Application::CGaleonApplication::getSingletonPtr()->getState())->getResourcesManager();
			resourcesManager->increaseResources(type, quantity);

			return true;
		}

		/** Reserva los recursos del tipo dado según la cantidad positiva indicada. 
		Con el flag allowPartial a true se permiten reservas parciales, esto es, se reserva todo lo que haya disponible aunque no llegue a lo solicitado.
		En finallyReserved se almacena la cantidad finalmente reservada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved){
			if (!isResourceTypeStored(type)){
				assert(false && "Resource not stored. Check wether it's stored before doing anything!");
				return 0;
			}

			if (quantity <= 0){
				assert(false && "Reserved quantity must be > 0");
				return false;
			}

			int aux = _reservedResources[type] + quantity;

			// Si se sobrepasa la cantidad disponible para reservar
			if (aux > getAvailableResources(type)){
				// Fallamos si no se deseaba reserva parcial
				if (!allowPartial){
					std::cout << "Discarded reserving " << quantity << " resources because there are only available " << getAvailableResources(type) << std::endl;
					finallyReserved = 0;
					return false;
				}

				// Si se permiten, reservamos hasta donde sea posible
				else{
					finallyReserved = getAvailableResources(type);
					_reservedResources[type] = _reservedResources[type] + finallyReserved;
					return true;
				}
			}

			_reservedResources[type] = aux;
			finallyReserved = quantity;

			return true;
		}

		/** Libera los recursos reservados del tipo dado según la cantidad positiva indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool freeReservedResources(ResourceType type, int quantity){
			if (!isResourceTypeStored(type)){
				assert(false && "Resource not stored. Check wether it's stored before doing anything!");
				return 0;
			}

			if (quantity <= 0){
				assert(false && "Freed quantity must be > 0");
				return false;
			}

			int aux = _reservedResources[type] - quantity;

			// Controlamos que no se libere más de lo que estaba reservado
			if (aux < 0){
				assert(false && "Can't free resources that weren't reserved");
				return false;
			}

			_reservedResources[type] = aux;
			return true;
		}

		/** Reclama los recursos previamente reservados del tipo dado en la cantidad indicada
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool claimReservedResources(ResourceType type, int quantity){
			if (!changeStoredResources(type, -quantity))
				return false;
			
			return freeReservedResources(type, quantity);
		}

		std::unordered_set<ResourceType>& getProvidedResources() {
			return _providedResources;
		}

		void registerStoredResourceType(std::unordered_set<ResourceType>& providedResources) {
			providedResources = _providedResources;
		}

		unsigned int getMaxResources() const{
			return _maxResources;
		}

		void setMaxResources(unsigned int maxResources){
			_maxResources = maxResources;
		}

	private:
		/** Cantidad de recursos almacenados indexados por tipo */
		std::unordered_map<ResourceType, int> _storedResources;

		/** Cantidad de recursos disponibles (i.e. no reservados) indexados por tipo */
		std::unordered_map<ResourceType, int> _reservedResources;

		/** Recursos que se proveen 
		TODO ¿Tiene sentido guardar y usar esta información en el ResourceBuilding? */
		std::unordered_set<ResourceType> _providedResources;

		/** Máxima cantidad que se puede almacenar de cada tipo */
		unsigned int _maxResources;
	};

}

#endif // SM_RESOURCE_BUILDING_DATA_