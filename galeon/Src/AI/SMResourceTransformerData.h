#ifndef SM_RESOURCE_TRANSFORMER_DATA_
#define SM_RESOURCE_TRANSFORMER_DATA_

namespace AI {
	class CSMResourceTransformerData {

	public:
		CSMResourceTransformerData() : _maxResources(0), _availableResources(0), _storedResources(0) {}

		virtual ~CSMResourceTransformerData() {}

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
		unsigned int _maxResources;
		int _availableResources;
		int _storedResources;
	};

}

#endif