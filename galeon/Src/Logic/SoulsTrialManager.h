//---------------------------------------------------------------------------
// SoulsTrialManager.h
//---------------------------------------------------------------------------

/**
@file SoulsTrialManager.h

Contiene la declaración del gestor del Juicio de Almas.

@see Logic::CSoulsTrialManager

@author Raúl Segura
@date Abril, 2016
*/

#ifndef __SOULS_TRIAL_MANAGER_H
#define __SOULS_TRIAL_MANAGER_H

#include <string>
#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

/**
Namespace que engloba la lógica del juego. Engloba desde el mapa lógico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para más información ver @ref LogicGroup).

@author David Llansó
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Manager singleton que centraliza la gestión del Juicio de Almas.
	Funcionamiento:

	1) Llegada periódica (entre 3 y 5 minutos) de grupos variables de
	   almas (entre 15 y 25 por grupo).

	2) Cada una de esas almas será asignada a una de las siguientes
	   categorías en función de su composición (todas contendrán 21
	   unidades de recursos sumando Crude 5-21, Coke 0-16 y Aether 0-5):
	     A) Unknown: ??
	     B) Heavy:   +11 Crude
	     C) Wasted:  +11 Coke
	     D) Light:   +3  Aether

	3) El porcentaje de almas reveladas, esto es, aquellas asignadas a
	   cualquier categoría distinta de Unknown, de cada grupo dependerá
	   del nivel del Juez (héroe encargado del Juicio de Almas).
	
	4) Todas estas almas permanecerán en espera hasta que el jugador
	   decida manualmente, en la pantalla del Juicio de Almas del
	   HellQuarters, cuántas almas de cada categoría quiere convertir
	   en trabajadores y cuántas quiere enviar a quemar.

	5) La cantidad exacta de recursos de cada alma (Crude y Coke para
	   las que se van a quemar, y Aether para los trabajadores) se
	   fijará en el momento de su creación de forma aleatoria,
	   respetando las restricciones especificadas en el punto 2.

	@ingroup logicGroup

	@author Raúl Segura
	@date Abril, 2016
	*/
	class CSoulsTrialManager
	{
	public:

		/**
		Categorías de almas en función de su composición aproximada.
		*/
		enum SoulsCategory
		{
			UNKNOWN,
			HEAVY,  // +11 Crude
			WASTED, // +11 Coke
			LIGHT,  // +3  Aether
			NONE
		};

		/**
		Devuelve la única instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CSoulsTrialManager *getSingletonPtr() { return _instance; }

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
		Registra esta clase en el contexto de Lua.
		IMPORTANTE: Llamar a este método desde CEventManager::luaRegister.
		*/
		static void luaRegister();

		/**
		Inicialización de los valores de configuración del manager,
		utilizando la información extraída de una entidad de mapa
		(Map::CEntity) leída previamente.

		@param entity Entidad de mapa con los valores de configuración
		leídos para el manager.
		*/
		void spawn(const Map::CEntity *managerInfo);

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Su función principal será la llegada periódica de grupos de
		almas.

		@param msecs milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned long msecs);

		/**
		Aumenta el nivel del Juez encargado del Juicio de Almas.
		*/
		void levelUp();

		/**
		Devuelve la lista de categorías de almas: Unknown, Heavy,
		Wasted y Light
		*/
		std::string* getSoulsCategories();

		/**
		Devuelve la cantidad de almas disponibles de cada categoría.
		*/
		unsigned int* getAvailableSouls() { return _souls; }

		/**
		Simula la llegada de un nuevo grupo de almas, asignando
		cada una de ellas a una categoría.
		*/
		void processNewGroupOfSouls();

		/**
		Función llamada cuando el jugador decida manualmente cuántas almas
		de cada categoría quiere convertir en trabajadores y enviar a
		quemar.
		<p>
		Llevará a cabo la creación de las almas, comprobando previamente
		que las cantidades recibidas son correctas con respecto al
		número de almas disponibles; y devolverá un vector con todas
		aquellas categorías para las que se haya solicitado una cantidad
		de almas superior al límite, a fin de poder indicar al jugador
		dónde está al error.

		@param numSoulsToWork número de almas de cada categoría a trabajar.
		@param numSoulsToBurn número de almas de cada categoría a quemar.
		@return vector de categorías para las que no hay suficientes almas
		disponibles.
		*/
		std::vector<SoulsCategory> createSouls(unsigned int numSoulsToWork[4], unsigned int numSoulsToBurn[4]);

		/**
		Funciones encargadas de determinar la composición exacta de cada
		alma, en función de su categoría, y devolver el valor exacto de
		cada tipo de recurso.

		@param soulCategory categoría del alma.
		@param soulCrude cantidad de Crude del alma.
		@param soulCoke cantidad de Coke del alma.
		@param soulAether cantidad de Éter del alma.
		@return true si la categoría recibida es distinta de NONE.
		*/
		bool calculateSoulEvil(SoulsCategory soulCategory, int& soulCrude, int& soulCoke);
		bool calculateSoulAether(SoulsCategory soulCategory, int& soulAether);

	protected:

		/**
		Constructor.
		*/
		CSoulsTrialManager();

		/**
		Destructor.
		*/
		virtual ~CSoulsTrialManager();

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

		/**
		Métodos auxiliares, llamados desde createSouls(), para la propia
		creación de ambos tipos de almas.
		*/
		void createSoulsToWork(unsigned int numSouls, SoulsCategory soulsCategory);
		void createSoulsToBurn(unsigned int numSouls, SoulsCategory soulsCategory);

		/**
		Método auxiliar para el cálculo de la cantidad exacta de recursos
		de cada tipo de un determinado alma.

		@param soulCategory categoría del alma.
		@param soulCrude cantidad de Crude del alma (entre 5 y 21).
		@param soulCoke cantidad de Coke del alma (entre 0 y 16).
		@param soulAether cantidad de Éter del alma (entre 0 y 5).
		*/
		void calculateSoulComposition(SoulsCategory soulCategory,
			int& soulCrude, int& soulCoke, int& soulAether);

	private:

		/**
		Única instancia de la clase.
		*/
		static CSoulsTrialManager *_instance;

		/**
		Cantidad de almas disponibles de cada categoría: Unknown, Heavy,
		Wasted y Light, respectivamente.
		*/
		unsigned int _souls[4];

		/**
		Rango de tiempo (ms) para la llegada periódica de grupos de almas.
		*/
		unsigned int _minSoulsGenerationTime = 10 * 1000;
		unsigned int _maxSoulsGenerationTime = 30 * 1000;

		unsigned int _timeForNextSoulsGeneration;
		unsigned int _timeSinceLastSoulsGeneration = 0;

		/**
		Número mínimo y máximo de almas presentes en cada nuevo grupo.
		*/
		unsigned int _minSoulsPerGroup = 15;
		unsigned int _maxSoulsPerGroup = 25;

		/**
		Nivel del Juez encargado de la revelación de las almas entrantes
		y probabilidad de revelar esas almas por cada nivel del juez.
		*/
		unsigned int _judgeLevel = 1;
		const unsigned int JUDGE_MAX_LEVEL = 3;
		const unsigned int REVEAL_SOUL_PROB = 30;

		/**
		Probabilidades acumuladas de asignar cada nueva alma a cualquier
		categoría distinta de Unknown.
		*/
		unsigned int _heavySoulProb  = 33;
		unsigned int _wastedSoulProb = 66;
		unsigned int _lightSoulProb  = 100;

	}; // class CSoulsTrialManager

} // namespace Logic

#endif // __SOULS_TRIAL_MANAGER_H