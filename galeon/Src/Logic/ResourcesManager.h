#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <unordered_map>
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
		PURE_EVIL,
		REFINED,
		AETHER,
		HADES_FAVOR
	};

	typedef std::unordered_map<ResourceType, float> CurrentResourcesMap;
	typedef std::unordered_map<ResourceType, unsigned int> MaxResourcesMap;

	class ResourcesManager
	{
	public:
		static ResourceType parseResourceType(const std::string& name);

		static ResourcesManager* getSingletonPtr();

		/**
		Inicializa la instancia y los recursos estáticos.
		@return true si la inicialización se hizo correctamente.
		*/
		static bool Init();

		/**
		Libera la propia instancia y los recursos estáticos.
		Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		* Devuelve la cantidad mostrada de recursos actuales de un tipo
		*/
		int getDisplayedResources(ResourceType type) const;
		
		/** 
		* Cambia la cantidad mostrada de recursos de un tipo sumando o restando la cantidad dada
		* NOTA: Este método sólo cambia el display de recursos, no añade ni quita físicamente recursos al sistema:
		* Ver @incrementResources y @decrementResources
		*/
		void changeDisplayedResources(ResourceType type, float num);

		/**
		* Devuelve la cantidad mostrada de recursos máximos de un tipo
		*/
		int getDisplayedMaxResources(ResourceType type) const;

		/**
		* Cambia la cantidad mostrada máxima de recursos de un tipo sumando o restando la cantidad dada
		*/
		void changeDisplayedMaxResources(ResourceType type, int num);

		/** 
		* Busca recursos disponibles (i.e. no reservados por un alma transportista, por ejemplo) de un tipo determinado entre los componentes de recursos dados.
		*
		* @param[in] type tipo de los recursos a buscar
		* @param[in] resourceBuildings vector de componentes CResourceBuilding sobre los que realizar la búsqueda
		* @param[out] totalAvailable cantidad total disponible encontrada
		* @return Mapa con la cifra de recursos disponibles de cada componente
		*/
		std::unordered_map<CResourceBuilding*, int> ResourcesManager::findResources(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalAvailable);

		/**
		* Busca recursos almacenables de un tipo determinado entre los componentes de recursos dados.
		*
		* @param[in] type tipo de los recursos a buscar
		* @param[in] resourceBuildings vector de componentes CResourceBuilding sobre los que realizar la búsqueda
		* @param[out] totalStorage cantidad total almacenable encontrada
		* @return Mapa con la cifra de recursos almacenables de cada componente
		*/
		std::unordered_map<CResourceBuilding*, int> ResourcesManager::findStorage(ResourceType type, const std::vector<CResourceBuilding*>& resourceBuildings, int& totalStorage);

		/**
		* Chequea si es posible decrementar y, opcionalmente, decrementa, una cantidad dada de recursos de un tipo en los edificios de recursos.
		* La implementación actual reparte el decremento entre todos los edificios de forma proporcional a la cantidad de recursos disponibles que tiene cada uno.
		* No hace falta llamar explícitamente a 'changeDisplayedResources' para cambiar el display porque los edificios internamente ya lo hacen.
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
		* No hace falta llamar explícitamente a 'changeDisplayedResources' para cambiar el display porque los edificios internamente ya lo hacen.
		*
		* @param[in] type tipo del recurso a incrementar
		* @param[in] increment cantidad a incrementar
		* @param[in] onlyCheck flag a true si sólo se desea comprobar si sería posible realizar el incremento
		* @param[in] allowPartial flag a true si se desea incrementar hasta lo que se pueda almacenar cuando no es posible incrementar todo lo solicitado
		* @param[out] finalIncrement cantidad finalmente incrementada
		* @return true si se pudo incrementar todo o parcialmente (en caso de allowPartial=true) lo solicitado / false en otro caso
		*/
		bool incrementResources(ResourceType type, int increment, bool onlyCheck, bool allowPartial, int& finalIncrement);

	protected:
		/*static const float MINERAL_GATHERING_SPEED;
		static const float GAS_GATHERING_SPEED;*/

		/**
		Constructor.
		*/
		ResourcesManager();

		/**
		Destructor.
		*/
		virtual ~ResourcesManager();

		/**
		Método encargado de registrar en el contexto de Lua todas aquellas
		clases y funciones necesarias en Lua.
		*/
		void luaRegister();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		estáticas.

		@return true si la inicialización se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberación de los recursos
		estáticos se hace en Release().
		*/
		void close();

		static ResourcesManager _instance;

		static CurrentResourcesMap ResourcesManager::createCurrentResourcesMap();
		static MaxResourcesMap ResourcesManager::createMaxResourcesMap();

		// Mapa genérico con la cantidad actual de recursos
		CurrentResourcesMap _currentResources;

		// Mapa genérico con la cantidad máxima de recursos
		MaxResourcesMap _maxResources;
	};

}

#endif //RESOURCEMANAGER_H_