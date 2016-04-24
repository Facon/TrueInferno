#ifndef SM_RESOURCE_TRANSFORMER_DATA_
#define SM_RESOURCE_TRANSFORMER_DATA_

namespace AI {
	class CSMResourceTransformerData {

	public:
		CSMResourceTransformerData() : _resourceType(Logic::ResourceType::NONE), _maxResources(0), _availableResources(0), _storedResources(0) {}

		virtual ~CSMResourceTransformerData() {}

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