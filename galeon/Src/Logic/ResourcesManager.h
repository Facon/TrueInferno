#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <string>
#include <map>

namespace Logic
{
	enum ResourceType
	{
		NONE = 0xFFFFFFFF,
		MINERAL,
		GAS,
		COKE,
		CRUDE,
		PURE,
		REFINED,
		AETHER,
		HADES_FAVOR
	};

	class ResourcesManager
	{
	protected:
		/*static const float MINERAL_GATHERING_SPEED;
		static const float GAS_GATHERING_SPEED;*/

		static ResourcesManager _instance;

	private:
		ResourcesManager() {
			_resources[ResourceType::MINERAL] = 100000;
			_resources[ResourceType::GAS] = 100000;
			_resources[ResourceType::COKE] = 0;
			_resources[ResourceType::CRUDE] = 0;
			_resources[ResourceType::PURE] = 0;
			_resources[ResourceType::REFINED] = 0;
			_resources[ResourceType::AETHER] = 0;
		}

		// Mapa genérico con los recursos
		std::map<ResourceType, float> _resources;

	public:
		static ResourceType parseResourceType(const std::string& name);

		static ResourcesManager& getSingleton()
		{ return _instance; }

		static ResourcesManager* getSingletonPtr()
		{ return &_instance; }

		/** Devuelve la cantidad actual de recursos del tipo consultado */
		int getResource(ResourceType type) const;
		
		/** Cambia la cantidad de recursos del tipo dado en la cantidad (positiva o negativa) indicada */
		void changeResources(ResourceType type, float num);

		/** Chequea si puede permitirse pagar la cantidad de recursos, cost, del tipo indicado, type.
		* Si allowPartial es true se permiten costes parciales y el coste final quse pagaría se devuelve en finalCost.
		* Devuelve true si se podría pagar la totalidad de los costes o parte (en caso de permitir costes parciales), y false en otro caso
		*/
		bool canAffordCost(ResourceType type, int cost, bool allowPartial, int& finalCost);

		/** Paga la cantidad de recursos, cost, del tipo indicado, type.
		* Si allowPartial es true se permiten costes parciales y el coste final pagado se devuelve en finalCost.
		* Devuelve true si se pudo pagar la totalidad de los costes o parte (en caso de permitir costes parciales), y false en otro caso.
		*/
		bool payCost(ResourceType type, int cost, bool allowPartial, int& finalCost);
	};

}

#endif //RESOURCEMANAGER_H_