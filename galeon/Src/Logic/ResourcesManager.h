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

		/**
		* Devuelve la cantidad actualmente mostrada de recursos del tipo consultado
		*/
		int getDisplayedResources(ResourceType type) const;
		
		/** 
		* Cambia la cantidad mostrada de recursos del tipo y cantidad dados.
		* NOTA: Este método sólo cambia el display de recursos, no añade ni quita físicamente recursos en el sistema:
		* Ver @incrementResources y @decrementResources
		*/
		void changeDisplayedResources(ResourceType type, float num);

		/** 
		* Busca recursos disponibles (i.e. no reservados por un alma transportista, por ejemplo) de un tipo determinado entre los componentes de recursos dados.
		*
		* @param[in] type tipo de los recursos a buscar
		* @param[in] resourceBuildings vector de componentes CResourceBuilding sobre los que realizar la búsqueda
		* @param[out] totalAvailable cantidad total disponible encontrada
		* @return Mapa con la cifra de recursos disponibles de cada componente
		*/
		std::map<CResourceBuilding*, int> ResourcesManager::findResources(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalAvailable);

		/**
		* Busca recursos almacenables de un tipo determinado entre los componentes de recursos dados.
		*
		* @param[in] type tipo de los recursos a buscar
		* @param[in] resourceBuildings vector de componentes CResourceBuilding sobre los que realizar la búsqueda
		* @param[out] totalStorage cantidad total almacenable encontrada
		* @return Mapa con la cifra de recursos almacenables de cada componente
		*/
		std::map<CResourceBuilding*, int> ResourcesManager::findStorage(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalStorage);

		/**
		* Chequea si es posible decrementar y, opcionalmente, decrementa, una cantidad dada de recursos de un tipo en los edificios de recursos.
		* La implementación actual reparte el decremento entre todos los edificios de forma proporcional a la cantidad de recursos disponibles que tiene cada uno.
		*
		* @param[in] type tipo del recurso a decrementar
		* @param[in] decrement cantidad a decrementar
		* @param[in] onlyCheck flag a true si sólo se desea comprobar si sería posible realizar el decremento (e.g. para comprobar si se puede pagar un coste de construcción)
		* @param[in] allowPartial flag a true si se desea decrementar hasta lo que haya disponible cuando no es posible decrementar todo lo solicitado (e.g. para pagar el coste de transformación de todos los recursos posibles)
		* @param[out] finalDecrement cantidad finalmente decrementada
		* @return true si se pudo decrementar todo o parcialmente (en caso de allowPartial=true) lo solicitado / false en otro caso
		*/
		bool decrementResources(ResourceType type, int decrement, bool onlyCheck, bool allowPartial, int& finalDecrement);

		/**
		* Chequea si es posible incrementar y, opcionalmente, incrementa, una cantidad dada de recursos de un tipo en los edificios de recursos.
		* La implementación actual reparte el incremento entre todos los edificios de forma proporcional a la cantidad de espacio almacenable que tiene cada uno.
		*
		* @param[in] type tipo del recurso a incrementar
		* @param[in] increment cantidad a incrementar
		* @param[in] onlyCheck flag a true si sólo se desea comprobar si sería posible realizar el incremento
		* @param[in] allowPartial flag a true si se desea incrementar hasta lo que se pueda almacenar cuando no es posible incrementar todo lo solicitado
		* @param[out] finalIncrement cantidad finalmente incrementada
		* @return true si se pudo incrementar todo o parcialmente (en caso de allowPartial=true) lo solicitado / false en otro caso
		*/
		bool incrementResources(ResourceType type, int increment, bool onlyCheck, bool allowPartial, int& finalIncrement);
	};

}

#endif //RESOURCEMANAGER_H_