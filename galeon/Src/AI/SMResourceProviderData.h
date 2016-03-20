#ifndef SM_RESOURCE_PROVIDER_DATA_H_
#define SM_RESOURCE_PROVIDER_DATA_H_

#include "Logic\ResourcesManager.h"
#include <vector>

using namespace Logic;

namespace AI {
	class CSMResourceProviderData {

	public:
		CSMResourceProviderData() : 
			_resourceType(ResourceType::NONE),
			_resourceQuantity(0),
			_target(EntityID::UNASSIGNED) {}

		virtual ~CSMResourceProviderData() {}

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

		/** Devuelve el objetivo al que tenemos que proveer */
		TEntityID getTarget() const{
			return _target;
		}

		/** Establece el objetivo al que tenemos que proveer */
		void setTarget(TEntityID target){
			_target = target;
		}
		
	private:
		/** Tipo de recurso asociado a la acción */
		ResourceType _resourceType;

		/** Cantidad de recurso asociada a la acción */
		unsigned int _resourceQuantity;

		/** Objetivo al que tenemos que proveer */
		TEntityID _target;
	};
}

#endif // SM_RESOURCE_PROVIDER_DATA_H_