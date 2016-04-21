#ifndef SM_RESOURCE_GATHERER_DATA_H_
#define SM_RESOURCE_GATHERER_DATA_H_

#include <map>

namespace AI {
	class CSMResourceGathererData {

	public:
		CSMResourceGathererData() {}

		virtual ~CSMResourceGathererData() {}

		/** 
		* Registra un tipo de recurso a recoger 
		*/
		void addGatheredResource(ResourceType type){
			_resourceStorage[type] = 0;
		}

		/**
		* Devuelve la capacidad restante actual de almacenaje del recurso dado 
		*/
		unsigned int getResourceStorage(ResourceType type) const {
			// Si no se almacena el recurso se devuelve 0 directamente
			if (_resourceStorage.count(type) == 0)
				return 0;

			return _resourceStorage.at(type);
		}

		/** 
		* Guarda la capacidad restante actual de almacenaje de un recurso.
		* @return true si pudo guardar, false si no
		*/
		bool setResourceStorage(ResourceType type, unsigned int storage){
			// Si no se almacena el recurso se devuelve falso
			if (_resourceStorage.count(type) == 0)
				return false;

			_resourceStorage[type] = storage;

			return true;
		}

		void clear(){
			_resourceStorage.clear();
		}

	private:
		/** Capacidad de almacenaje restante actual para cada tipo de recurso */
		std::map<ResourceType, int> _resourceStorage;
	};

}

#endif