#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <string>
#include <map>
#include <vector>

namespace Logic
{
	class CResourceBuilding;

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
			_resources[ResourceType::MINERAL] = 0;
			_resources[ResourceType::GAS] = 0;
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

		/** Busca recursos del tipo dado entre todos los edificios.
		Devuelve un mapa con las cantidades de recursos de cada edificio */
		std::map<CResourceBuilding*, int> ResourcesManager::findResources(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalAvailable);

		/** Chequea y paga o sólo chequea la cantidad de recursos, cost, del tipo indicado, type.
		* Si allowPartial es true se permiten costes parciales y el coste final pagado se devuelve en finalCost.
		* Devuelve true si se pudo pagar la totalidad de los costes o parte (en caso de permitir costes parciales), y false en otro caso.
		*/
		bool payCost(ResourceType type, int cost, bool onlyCheck, bool allowPartial, int& finalCost);
	};

}

#endif //RESOURCEMANAGER_H_