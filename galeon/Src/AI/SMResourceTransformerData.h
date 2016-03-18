#ifndef SM_RESOURCE_TRANSFORMER_DATA_
#define SM_RESOURCE_TRANSFORMER_DATA_

namespace AI {
	class CSMResourceTransformerData {

	public:
		CSMResourceTransformerData() : _maxResources(0), _availableResources(0) {}

		virtual ~CSMResourceTransformerData() {}

		unsigned int getMaxResources() const{
			return _maxResources;
		}

		void setMaxResources(unsigned int maxResources){
			_maxResources = maxResources;
		}

		unsigned int getAvailableResources() const{
			return _availableResources;
		}

		void setAvailableResources(unsigned int availableResources){
			_availableResources = availableResources;
		}

	private:
		unsigned int _maxResources;
		unsigned int _availableResources;

	};

}

#endif