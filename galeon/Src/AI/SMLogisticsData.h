#ifndef SM_LOGISTICS_DATA_
#define SM_LOGISTICS_DATA_

#include "Logic\ResourcesManager.h"
#include <vector>

namespace AI {
	class CSMLogisticsData {

	public:
		CSMLogisticsData() : 
			_resourceType(ResourceType::NONE),
			_resourceQuantity(0) {}

		virtual ~CSMLogisticsData() {}

		LogisticsAction getAction() const{
			return _action;
		}

		void setAction(LogisticsAction action){
			_action = action;
		}

		ResourceType getResourceType() const{
			return _resourceType;
		}

		void setResourceType(ResourceType resourceType){
			_resourceType = resourceType;
		}

		unsigned int getResourceQuantity() const{
			return _resourceQuantity;
		}

		void setResourceQuantity(unsigned int resourceQuantity){
			_resourceQuantity = resourceQuantity;
		}

		void clearProviderMessages(){
			_msgs.clear();
		}

		void addProviderMessage(LogisticsMessage msg){
			_msgs.push_back(msg);
		}

		std::vector<LogisticsMessage> getProviderMessages(){
			return _msgs;
		}
		
	private:
		LogisticsAction _action;
		ResourceType _resourceType;
		unsigned int _resourceQuantity;

		/** Vector con los mensajes recibidos con información de recursos */
		std::vector<LogisticsMessage> _msgs;
	};
}

#endif