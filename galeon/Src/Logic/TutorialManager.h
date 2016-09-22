//---------------------------------------------------------------------------
// TutorialManager.h
//---------------------------------------------------------------------------

/**
@file TutorialManager.h

Contiene la declaración del gestor del tutorial.

@see Logic::CTutorialManager

@author Raúl Segura
@date Septiembre, 2016
*/

#ifndef __TUTORIAL_MANAGER_H
#define __TUTORIAL_MANAGER_H

#include "Logic/Events/Event.h"
#include "Logic/Entity/MessageHandler.h"

#include <OgreRoot.h>

namespace Logic
{
	enum BuildingType;
}

namespace GUI
{
	class SideBarUI;
}

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
	Conjunto de etapas o fases del tutorial.
	*/
	enum TutorialStage
	{
		// Tutorial aún no iniciado
		NOT_STARTED,
		// Etapas del tutorial (en orden):
		HELLQUARTERS_CLICK,             // Bienvenida y click en el HQ.
		HELLQUARTERS_CLICK_SOULS_TRIAL, // Almas y click en Juicio de Almas.
		SOULS_TRIAL,                    // Juicio de Almas y tipos.
		REFINED_EVIL_AS_OBJ_1,          // El objetivo final es refinar el Mal y obtener Refined Evil.
		REFINED_EVIL_AS_OBJ_2,          // El objetivo final es refinar el Mal y obtener Refined Evil.
		ROAD,                           // Carreteras y necesidad de conectar edificios.
		MINE,                           // Mine: localización y Mineral.
		GAS_PLANT,                      // GasPlant: localización y Gas.
		FURNACE,                        // Furnace: Crude Evil y Coke.
		EVILWORKS,                      // Evilworks: Pure Evil.
		REFINERY,                       // Refinery: Refined Evil, Evilator y Hades Favor.
		POWER_GENERATOR,                // Power Generator: Power.
		WAREHOUSE,                      // Warehouse: almacenamiento de recursos.
		// Tutorial finalizado
		FINISHED,
		// Tutorial desactivado
		SKIPPED
	};

	/**
	Manager singleton que centraliza la gestión del tutorial. El salto de cada
	etapa a la siguiente será realizado a través de la llegada de mensajes:
	
	1) Al recibir ciertos tipos de mensajes (por ejemplo, aquellos de "Edificio construido"),
	   que son condición para superar determinadas fases del tutorial, se intentará avanzar
	   a la siguiente etapa.

	2) Así, para cada uno de esos mensajes, se comprobará si, efectivamente, el tutorial está
	   activo y la siguiente fase coincide con aquella a la que se intenta saltar.

	@ingroup logicGroup

	@author Raúl Segura
	@date Septiembre, 2016
	*/
	class CTutorialManager : public MessageHandler
	{
	public:
		/**
		Devuelve la única instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CTutorialManager *getSingletonPtr() { return _instance; }

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
		Inicialización de los valores de configuración del manager,
		utilizando la información extraída de una entidad de mapa
		(Map::CEntity) leída previamente.

		@param entity Entidad de mapa con los valores de configuración
		leídos para el manager.
		*/
		void spawn(const Map::CEntity *managerInfo);

		/**
		Inicio del tutorial.
		Llamado desde Logic::CServer::tick.
		*/
		void start();

		/**
		Notificación de botón "OK" o "SKIP" pulsado en un evento del
		tutorial.
		*/
		void buttonOK();
		void buttonSKIP();

		/**
		Notificaciones varias relevantes para el avance del tutorial.
		*/
		void buildingSelected(std::string buildingName); // Edificio seleccionado
		void soulsTrialSelected(); // Juicio de Almas seleccionado
		void soulsTrialCompleted(); // Juicio de Almas completado satisfactoriamente
		void roadPlaced(); // Carretera construida
		void buildingPlaced(BuildingType buildingType); // Edificio construido

		/**
		Manejo de mensajes, tiene que manejar todos los tipos de mensajes sin excepción.
		*/
		bool HandleMessage(const Message& msg);
		bool HandleMessage(const TransformMessage& msg);
		bool HandleMessage(const PositionMessage& msg);
		bool HandleMessage(const RotationMessage& msg);
		bool HandleMessage(const DimensionsMessage& msg);
		bool HandleMessage(const ColorMessage& msg);
		bool HandleMessage(const MaterialMessage& msg);
		bool HandleMessage(const AnimationMessage& msg);
		bool HandleMessage(const ControlMessage& msg);
		bool HandleMessage(const PhysicMessage& msg);
		bool HandleMessage(const TouchMessage& msg);
		bool HandleMessage(const DamageMessage& msg);
		bool HandleMessage(const WorkerMessage& msg);
		bool HandleMessage(const WalkSoulPathMessage& msg);
		bool HandleMessage(const HellQuartersMessage& msg);
		bool HandleMessage(const MovePlaceableMessage& msg);
		bool HandleMessage(const SoulSenderMessage& msg);
		bool HandleMessage(const SoulMessage& msg);
		bool HandleMessage(const CheckValidPositionPlaceableMessage& msg);
		bool HandleMessage(const SoulBurnMessage& msg);
		bool HandleMessage(const ResourceMessage& msg);
		bool HandleMessage(const GetCostPlaceableMessage& msg);
		bool HandleMessage(const LogisticsMessage& msg);
		bool HandleMessage(const ToggleMessage& msg);
		bool HandleMessage(const IconMessage& msg);
		bool HandleMessage(const PowerMessage& msg);
		bool HandleMessage(const ConsumptionMessage& msg);
		bool HandleMessage(const SoundMessage& msg);
		bool HandleMessage(const ParticleMessage& msg);

	protected:

		/**
		Constructor.
		*/
		CTutorialManager();

		/**
		Destructor.
		*/
		virtual ~CTutorialManager();

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
		Comprueba, dada la etapa de entrada (que representa la etapa del
		tutorial finalizada en base a un cierto tipo de mensaje recibido),
		si se debe ejecutar la siguiente fase del tutorial y llama a la
		función encargada de hacerlo en cada caso.

		@return true si se debía avanzar a la etapa posterior y el cambio
		se hizo correctamente.
		*/
		bool nextStage(TutorialStage finishedStage);

		/**
		Conjunto de procedimientos para llevar a cabo los ajustes a realizar
		al inicio de cada etapa.
		*/
		void startStageHellQuartersClick();
		void startStageHellQuartersClickSoulsTrial();
		void startStageSoulsTrial();
		void startStageRefinedEvil1();
		void startStageRefinedEvil2();
		void startStageRoad();
		void startStageMine();
		void startStageGasPlant();
		void startStageFurnace();
		void startStageEvilworks();
		void startStageRefinery();
		void startStagePowerGenerator();
		void startStageWarehouse();

		/**
		Fin del tutorial.
		*/
		void endTutorial();

	private:

		/**
		Única instancia de la clase.
		*/
		static CTutorialManager *_instance;

		/**
		Tutorial activo, aún no finalizado ni deshabilitado.
		*/
		bool _active = true;

		/**
		Etapa actual en la que se encuentra el tutorial.
		*/
		TutorialStage _currentStage = TutorialStage::NOT_STARTED;

		/**
		Referencia a la instancia única de la clase.
		*/
		GUI::SideBarUI *_sideBarUI = nullptr;

		/**
		TODO Debería cogerlas del BuildingManager
		*/
		Ogre::Vector3 hellquartersPosition = Ogre::Vector3(24, 0, 23);
		Ogre::Vector3 evilatorPosition = Ogre::Vector3(18, 0, 20);

	}; // class CTutorialManager

} // namespace Logic

#endif // __TUTORIAL_MANAGER_H