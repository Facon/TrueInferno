#ifndef SM_RESOURCE_GATHERER_DATA_H_
#define SM_RESOURCE_GATHERER_DATA_H_

#include <map>

namespace AI {
	class CSMResourceGathererData {

	public:
		CSMResourceGathererData() : _resourceType(Logic::ResourceType::NONE), _maxResources(0), _availableResources(0), _storedResources(0) {}

		virtual ~CSMResourceGathererData() {}

		/** 
		* Registra un tipo de recurso a recoger 
		*
		void addGatheredResource(ResourceType type){
			_resourceStorage[type] = 0;
		}

		/**
		* Devuelve la capacidad restante actual de almacenaje del recurso dado 
		*
		unsigned int getResourceStorage(ResourceType type) const {
			// Si no se almacena el recurso se devuelve 0 directamente
			if (_resourceStorage.count(type) == 0)
				return 0;

			return _resourceStorage.at(type);
		}

		/** 
		* Guarda la capacidad restante actual de almacenaje de un recurso.
		* @return true si pudo guardar, false si no
		*
		bool setResourceStorage(ResourceType type, unsigned int storage){
			// Si no se almacena el recurso se devuelve falso
			if (_resourceStorage.count(type) == 0)
				return false;

			_resourceStorage[type] = storage;

			return true;
		}

		/** 
		* Devuelve Cantidad de recursos que se va a solicitar
		*/
		unsigned int getResourcesToAsk(ResourceType type) const {
			//return getResourceStorage(type);

			assert(type == _resourceType && "Can't ask resources from an unregistered type");
			return _maxResources - _storedResources;
		}

		unsigned int getResourceType() const{
			return _resourceType;
		}

		void setResourceType(Logic::ResourceType resourceType){
			_resourceType = resourceType;
		}

		unsigned int getMaxResources() const{
			return _maxResources;
		}

		void setMaxResources(unsigned int maxResources){
			_maxResources = maxResources;
		}

		int getAvailableResources() const{
			return _availableResources;
		}

		void setAvailableResources(int availableResources){
			_availableResources = availableResources;
		}

		int getStoredResources() const{
			return _storedResources;
		}

		void setStoredResources(int storedResources){
			_storedResources = storedResources;
		}

	private:
		/** Capacidad de almacenaje restante actual para cada tipo de recurso */
		//std::map<ResourceType, int> _resourceStorage;

		/** Tipo de recurso que se recoge */
		ResourceType _resourceType;

		/** Cantidad máxima de recurso que se puede almacenar */
		unsigned int _maxResources;

		/** Cantidad disponible de recurso teniendo en cuenta las reservas */
		int _availableResources;

		/** Cantidad almacenada realmente de recurso */
		int _storedResources;
	};

}

#endif