//---------------------------------------------------------------------------
// SoulsTrialManager.h
//---------------------------------------------------------------------------

/**
@file SoulsTrialManager.h

Contiene la declaraci�n del gestor del Juicio de Almas.

@see Logic::CSoulsTrialManager

@author Ra�l Segura
@date Abril, 2016
*/

#ifndef __SOULS_TRIAL_MANAGER_H
#define __SOULS_TRIAL_MANAGER_H

#include <string>
#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Map
{
	class CEntity;
}

/**
Namespace que engloba la l�gica del juego. Engloba desde el mapa l�gico
contenedor de todas las entidades del juego hasta las propias entidades,
sus componentes, mensajes, factorias de entidades y componentes, etc.
(para m�s informaci�n ver @ref LogicGroup).

@author David Llans�
@date Agosto, 2010
*/
namespace Logic
{
	/**
	Manager singleton que centraliza la gesti�n del Juicio de Almas.
	Funcionamiento:

	1) Llegada peri�dica (entre 3 y 5 minutos) de grupos variables de
	   almas (entre 15 y 25 por grupo).

	2) Cada una de esas almas ser� asignada a una de las siguientes
	   categor�as en funci�n de su composici�n (todas contendr�n 21
	   unidades de recursos sumando Crude 5-21, Coke 0-16 y Aether 0-5):
	     A) Unknown: ??
	     B) Heavy:   +11 Crude
	     C) Wasted:  +11 Coke
	     D) Light:   +3  Aether

	3) El porcentaje de almas reveladas, esto es, aquellas asignadas a
	   cualquier categor�a distinta de Unknown, de cada grupo depender�
	   del nivel del Juez (h�roe encargado del Juicio de Almas).
	
	4) Todas estas almas permanecer�n en espera hasta que el jugador
	   decida manualmente, en la pantalla del Juicio de Almas del
	   HellQuarters, cu�ntas almas de cada categor�a quiere convertir
	   en trabajadores y cu�ntas quiere enviar a quemar.

	5) La cantidad exacta de recursos de cada alma (Crude y Coke para
	   las que se van a quemar, y Aether para los trabajadores) se
	   fijar� en el momento de su creaci�n de forma aleatoria,
	   respetando las restricciones especificadas en el punto 2.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Abril, 2016
	*/
	class CSoulsTrialManager
	{
	public:

		/**
		Categor�as de almas en funci�n de su composici�n aproximada.
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
		Devuelve la �nica instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CSoulsTrialManager *getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia y los recursos est�ticos.
		@return true si la inicializaci�n se hizo correctamente.
		*/
		static bool Init();

		/**
		Libera la propia instancia y los recursos est�ticos.
		Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Registra esta clase en el contexto de Lua.
		IMPORTANTE: Llamar a este m�todo desde CEventManager::luaRegister.
		*/
		static void luaRegister();

		/**
		Inicializaci�n de los valores de configuraci�n del manager,
		utilizando la informaci�n extra�da de una entidad de mapa
		(Map::CEntity) le�da previamente.

		@param entity Entidad de mapa con los valores de configuraci�n
		le�dos para el manager.
		*/
		void spawn(const Map::CEntity *managerInfo);

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Su funci�n principal ser� la llegada peri�dica de grupos de
		almas.

		@param msecs milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned long msecs);

		/**
		Aumenta el nivel del Juez encargado del Juicio de Almas.
		*/
		void levelUp();

		/**
		Devuelve la lista de categor�as de almas: Unknown, Heavy,
		Wasted y Light
		*/
		std::string* getSoulsCategories();

		/**
		Devuelve la cantidad de almas disponibles de cada categor�a.
		*/
		unsigned int* getAvailableSouls() { return _souls; }

		/**
		Simula la llegada de un nuevo grupo de almas, asignando
		cada una de ellas a una categor�a.
		*/
		void processNewGroupOfSouls();

		/**
		Funci�n llamada cuando el jugador decida manualmente cu�ntas almas
		de cada categor�a quiere convertir en trabajadores y enviar a
		quemar.
		<p>
		Llevar� a cabo la creaci�n de las almas, comprobando previamente
		que las cantidades recibidas son correctas con respecto al
		n�mero de almas disponibles; y devolver� un vector con todas
		aquellas categor�as para las que se haya solicitado una cantidad
		de almas superior al l�mite, a fin de poder indicar al jugador
		d�nde est� al error.

		@param numSoulsToWork n�mero de almas de cada categor�a a trabajar.
		@param numSoulsToBurn n�mero de almas de cada categor�a a quemar.
		@return vector de categor�as para las que no hay suficientes almas
		disponibles.
		*/
		std::vector<SoulsCategory> createSouls(unsigned int numSoulsToWork[4], unsigned int numSoulsToBurn[4]);

		/**
		Funciones encargadas de determinar la composici�n exacta de cada
		alma, en funci�n de su categor�a, y devolver el valor exacto de
		cada tipo de recurso.

		@param soulCategory categor�a del alma.
		@param soulCrude cantidad de Crude del alma.
		@param soulCoke cantidad de Coke del alma.
		@param soulAether cantidad de �ter del alma.
		@return true si la categor�a recibida es distinta de NONE.
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
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones
		est�ticas.

		@return true si la inicializaci�n se hizo correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar
		los recursos de la propia instancia, la liberaci�n de los recursos
		est�ticos se hace en Release().
		*/
		void close();

		/**
		M�todos auxiliares, llamados desde createSouls(), para la propia
		creaci�n de ambos tipos de almas.
		*/
		void createSoulsToWork(unsigned int numSouls, SoulsCategory soulsCategory);
		void createSoulsToBurn(unsigned int numSouls, SoulsCategory soulsCategory);

		/**
		M�todo auxiliar para el c�lculo de la cantidad exacta de recursos
		de cada tipo de un determinado alma.

		@param soulCategory categor�a del alma.
		@param soulCrude cantidad de Crude del alma (entre 5 y 21).
		@param soulCoke cantidad de Coke del alma (entre 0 y 16).
		@param soulAether cantidad de �ter del alma (entre 0 y 5).
		*/
		void calculateSoulComposition(SoulsCategory soulCategory,
			int& soulCrude, int& soulCoke, int& soulAether);

	private:

		/**
		�nica instancia de la clase.
		*/
		static CSoulsTrialManager *_instance;

		/**
		Cantidad de almas disponibles de cada categor�a: Unknown, Heavy,
		Wasted y Light, respectivamente.
		*/
		unsigned int _souls[4];

		/**
		Rango de tiempo (ms) para la llegada peri�dica de grupos de almas.
		*/
		unsigned int _minSoulsGenerationTime = 10 * 1000;
		unsigned int _maxSoulsGenerationTime = 30 * 1000;

		unsigned int _timeForNextSoulsGeneration;
		unsigned int _timeSinceLastSoulsGeneration = 0;

		/**
		N�mero m�nimo y m�ximo de almas presentes en cada nuevo grupo.
		*/
		unsigned int _minSoulsPerGroup = 15;
		unsigned int _maxSoulsPerGroup = 25;

		/**
		Nivel del Juez encargado de la revelaci�n de las almas entrantes
		y probabilidad de revelar esas almas por cada nivel del juez.
		*/
		unsigned int _judgeLevel = 1;
		const unsigned int JUDGE_MAX_LEVEL = 3;
		const unsigned int REVEAL_SOUL_PROB = 30;

		/**
		Probabilidades acumuladas de asignar cada nueva alma a cualquier
		categor�a distinta de Unknown.
		*/
		unsigned int _heavySoulProb  = 33;
		unsigned int _wastedSoulProb = 66;
		unsigned int _lightSoulProb  = 100;

	}; // class CSoulsTrialManager

} // namespace Logic

#endif // __SOULS_TRIAL_MANAGER_H