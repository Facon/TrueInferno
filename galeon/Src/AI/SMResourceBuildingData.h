#ifndef SM_RESOURCE_BUILDING_DATA_
#define SM_RESOURCE_BUILDING_DATA_

#include <unordered_map>
#include <unordered_set>

namespace AI {
	class CSMResourceBuildingData {

	public:
		CSMResourceBuildingData() : _maxResources(0) {
			_storedResources.clear();
			_providedResources.clear();
		}

		virtual ~CSMResourceBuildingData() {
			_storedResources.clear();
			_providedResources.clear();
		}

		std::unordered_map<ResourceType, int>& getStoredResources() {
			return _storedResources;
		}

		void setStoredResources(std::unordered_map<ResourceType, int>& storedResources) {
			storedResources = _storedResources;
		}

		std::unordered_set<ResourceType>& getProvidedResources() {
			return _providedResources;
		}

		void setProvidedResources(std::unordered_set<ResourceType>& providedResources) {
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

		/** Recursos que se proveen */
		std::unordered_set<ResourceType> _providedResources;

		/** Máxima cantidad que se puede almacenar de cada tipo */
		unsigned int _maxResources;
	};

}

#endif // SM_RESOURCE_BUILDING_DATA_