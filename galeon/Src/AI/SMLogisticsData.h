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

		/** Devuelve la acción solicitada */
		LogisticsAction getAction() const{
			return _action;
		}

		/** Establece la acción solicitada */
		void setAction(LogisticsAction action){
			_action = action;
		}

		/** Devuelve el tipo de recurso asociado a la acción */
		ResourceType getResourceType() const{
			return _resourceType;
		}

		/** Establece el tipo de recurso asociado a la acción */
		void setResourceType(ResourceType resourceType){
			_resourceType = resourceType;
		}

		/** Devuelve la cantidad de recurso asociada a la acción */
		unsigned int getResourceQuantity() const{
			return _resourceQuantity;
		}
		
		/** Establece la cantidad de recurso asociada a la acción */
		void setResourceQuantity(unsigned int resourceQuantity){
			_resourceQuantity = resourceQuantity;
		}

		/** Limpia el vector con los mensajes recibidos de los proveedores de recursos */
		void clearProviderMessages(){
			_providerMessages.clear();
		}

		/** Añade una entrada al vector con los mensajes recibidos de los proveedores de recursos */
		void addProviderMessage(ResourceMessage msg){
			_providerMessages.push_back(msg);
		}

		/** Devuelve el vector con los mensajes recibidos de los proveedores de recursos */
		std::vector<ResourceMessage> getProviderMessages(){
			return _providerMessages;
		}
		
	private:
		/** Acción solicitada */
		LogisticsAction _action;

		/** Tipo de recurso asociado a la acción */
		ResourceType _resourceType;

		/** Cantidad de recurso asociada a la acción */
		unsigned int _resourceQuantity;

		/** Vector con los mensajes recibidos con información de recursos */
		std::vector<ResourceMessage> _providerMessages;
	};
}

#endif