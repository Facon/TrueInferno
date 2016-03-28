#ifndef SM_RESOURCE_DEMANDER_DATA_H_
#define SM_RESOURCE_DEMANDER_DATA_H_

#include "Logic\ResourcesManager.h"
#include <vector>

using namespace Logic;

namespace AI {
	class CSMResourceDemanderData {

	public:
		CSMResourceDemanderData() : 
			_resourceType(ResourceType::NONE),
			_resourceQuantity(0) {}

		virtual ~CSMResourceDemanderData() {}

		/** Devuelve el tipo de recurso asociado a la acción */
		ResourceType getResourceType() const{
			return _resourceType;
		}

		/** Establece el tipo de recurso asociado a la acción */
		void setResourceType(ResourceType resourceType){
			_resourceType = resourceType;
		}

		/** Devuelve la cantidad de recurso asociada a la acción */
		int getResourceQuantity() const{
			return _resourceQuantity;
		}
		
		/** Establece la cantidad de recurso asociada a la acción */
		void setResourceQuantity(int resourceQuantity){
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
		std::vector<ResourceMessage>& getProviderMessages(){
			return _providerMessages;
		}
		
	private:
		/** Tipo de recurso asociado a la acción */
		ResourceType _resourceType;

		/** Cantidad de recurso asociada a la acción */
		int _resourceQuantity;

		/** Vector con los mensajes recibidos con información de recursos de los proveedores */
		std::vector<ResourceMessage> _providerMessages;
	};
}

#endif // SM_RESOURCE_DEMANDER_DATA_H_