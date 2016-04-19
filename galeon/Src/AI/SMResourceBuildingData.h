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
		CSMResourceBuildingData() : _maxResources(0), _initialResources(0) {}

		virtual ~CSMResourceBuildingData() {
			_storedResources.clear();
			_reservedResources.clear();
			_providedResources.clear();
		}

		/** Registra el tipo de recurso dado para ser almacenado */
		void registerStoredResourceType(ResourceType type);

		/** Devuelve true o false según si el tipo de recurso es almacenable en esta entidad o no */
		bool isResourceTypeStored(ResourceType type);

		/** Devuelve la cantidad de recursos almacenados del tipo dado */
		int getStoredResources(ResourceType type);

		/** Devuelve la cantidad de recursos disponibles del tipo dado = Recursos existentes menos los reservados */
		int getAvailableResources(ResourceType type);

		/** Modifica los recursos del tipo dado según la cantidad positiva o negativa indicada.
		Devuelve true o false según si la operación se realizó correctamente o no */
		bool changeStoredResources(ResourceType type, int quantity);

		/** Reserva los recursos del tipo dado según la cantidad positiva indicada. 
		Con el flag allowPartial a true se permiten reservas parciales, esto es, se reserva todo lo que haya disponible aunque no llegue a lo solicitado.
		En finallyReserved se almacena la cantidad finalmente reservada.
		Devuelve true o false según si la operación se realizó correctamente o no */
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

		/** Getter del máximo de recursos */
		unsigned int getMaxResources() const;

		/** Setter del máximo de recursos */
		void setMaxResources(unsigned int maxResources);

		/** Getter de la cantidad inicial de recursos */
		unsigned int getInitialResources() const;

		/** Setter de la cantidad inicial de recursos */
		void setInitialResources(unsigned int initialResources);

		/** Inicializa los recursos de partida */
		void initResources();

		/** Limpia los recursos almacenados */
		void cleanResources();

	private:
		/** Cantidad de recursos almacenados indexados por tipo */
		std::unordered_map<ResourceType, int> _storedResources;

		/** Cantidad de recursos disponibles (i.e. no reservados) indexados por tipo */
		std::unordered_map<ResourceType, int> _reservedResources;

		/** Recursos que se proveen */
		std::unordered_set<ResourceType> _providedResources;

		/** Máxima cantidad que se puede almacenar de cada tipo */
		unsigned int _maxResources;

		/** Cantidad inicial de cada tipo */
		unsigned int _initialResources;
	};
}

#endif // SM_RESOURCE_BUILDING_DATA_H_