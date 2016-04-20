#ifndef SM_RESOURCE_BUILDING_DATA_H_
#define SM_RESOURCE_BUILDING_DATA_H_

#include <unordered_map>
#include <unordered_set>

namespace Logic {
	enum ResourceType;
}

using namespace Logic;

namespace AI {
	class CSMResourceBuildingData {

	public:
		CSMResourceBuildingData() {}

		virtual ~CSMResourceBuildingData() {
			_storedResources.clear();
			_reservedResources.clear();
			_maxResources.clear();
			_providedResources.clear();
		}

		/** 
		Registra el tipo de recurso a ser almacenado con su cantidad inicial y su máximo.
		*/
		void registerStoredResourceType(ResourceType type, int initialResources, int maxResources);

		/** Devuelve true o false según si el tipo de recurso es almacenable en esta entidad o no */
		bool isResourceTypeStored(ResourceType type) const;

		/** Devuelve la cantidad de recursos almacenados del tipo dado */
		int getStoredResources(ResourceType type) const;

		/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
		int getAvailableResources(ResourceType type) const;

		/** Getter del máximo de recursos */
		unsigned int getMaxResources(ResourceType type) const;

		/**
		* Modifica los recursos almacenados del tipo dado según la cantidad positiva o negativa indicada.
		* Devuelve true o false según si la operación se realizó correctamente o no
		*/
		bool changeStoredResources(ResourceType type, int quantity);

		/**
		* Modifica los recursos máximos del tipo dado según la cantidad positiva o negativa indicada.
		* Devuelve true o false según si la operación se realizó correctamente o no
		*/
		bool changeMaxResources(ResourceType type, int quantity);

		/**
		* Reserva los recursos del tipo dado según la cantidad positiva indicada. 
		* 
		* @param[in] type tipo de los recursos a reservar
		* @param[in] quantity cantidad de recursos a reservar
		* @param[in] allowPartial flag a true para permite reservas parciales, esto es, reservar todo lo que haya disponible aunque no se llegue a lo solicitado
		* @param[out] finallyReserved cantidad finalmente reservada
		* @return true o false según si la operación total o parcial se realizó correctamente o no 
		*/
		bool reserveResources(ResourceType type, int quantity, bool allowPartial, int& finallyReserved);

		/** Libera los recursos reservados del tipo dado según la cantidad positiva indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool freeReservedResources(ResourceType type, int quantity);

		/** Reclama los recursos previamente reservados del tipo dado en la cantidad indicada
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool claimReservedResources(ResourceType type, int quantity);

		/** Devuelve la lista de recursos que la entidad provee */
		std::unordered_set<ResourceType>& getProvidedResources();

		/** Registra un tipo */
		//void registerStoredResourceType(std::unordered_set<ResourceType>& providedResources);

		/** Desactiva el objeto limpiando y notificando todo lo que sea necesario */
		void deactivate();

	private:
		/** Cantidad de recursos almacenados indexados por tipo */
		std::unordered_map<ResourceType, int> _storedResources;

		/** Cantidad máxima de recursos almacenados indexados por tipo */
		std::unordered_map<ResourceType, int> _maxResources;
		
		/** Cantidad de recursos disponibles (i.e. no reservados) indexados por tipo */
		std::unordered_map<ResourceType, int> _reservedResources;

		/** Recursos que se proveen */
		std::unordered_set<ResourceType> _providedResources;
	};
}

#endif // SM_RESOURCE_BUILDING_DATA_H_