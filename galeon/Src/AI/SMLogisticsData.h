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

		/** Devuelve la acci�n solicitada */
		LogisticsAction getAction() const{
			return _action;
		}

		/** Establece la acci�n solicitada */
		void setAction(LogisticsAction action){
			_action = action;
		}

		/** Devuelve el tipo de recurso asociado a la acci�n */
		ResourceType getResourceType() const{
			return _resourceType;
		}

		/** Establece el tipo de recurso asociado a la acci�n */
		void setResourceType(ResourceType resourceType){
			_resourceType = resourceType;
		}

		/** Devuelve la cantidad de recurso asociada a la acci�n */
		unsigned int getResourceQuantity() const{
			return _resourceQuantity;
		}
		
		/** Establece la cantidad de recurso asociada a la acci�n */
		void setResourceQuantity(unsigned int resourceQuantity){
			_resourceQuantity = resourceQuantity;
		}

		/** Limpia el vector con los mensajes recibidos de los proveedores de recursos */
		void clearProviderMessages(){
			_providerMessages.clear();
		}

		/** A�ade una entrada al vector con los mensajes recibidos de los proveedores de recursos */
		void addProviderMessage(ResourceMessage msg){
			_providerMessages.push_back(msg);
		}

		/** Devuelve el vector con los mensajes recibidos de los proveedores de recursos */
		std::vector<ResourceMessage> getProviderMessages(){
			return _providerMessages;
		}
		
	private:
		/** Acci�n solicitada */
		LogisticsAction _action;

		/** Tipo de recurso asociado a la acci�n */
		ResourceType _resourceType;

		/** Cantidad de recurso asociada a la acci�n */
		unsigned int _resourceQuantity;

		/** Vector con los mensajes recibidos con informaci�n de recursos */
		std::vector<ResourceMessage> _providerMessages;
	};
}

#endif