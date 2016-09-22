//---------------------------------------------------------------------------
// TutorialManager.h
//---------------------------------------------------------------------------

/**
@file TutorialManager.h

Contiene la declaraci�n del gestor del tutorial.

@see Logic::CTutorialManager

@author Ra�l Segura
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
	Conjunto de etapas o fases del tutorial.
	*/
	enum TutorialStage
	{
		// Tutorial a�n no iniciado
		NOT_STARTED,
		// Etapas del tutorial (en orden):
		HELLQUARTERS_CLICK,             // Bienvenida y click en el HQ.
		HELLQUARTERS_CLICK_SOULS_TRIAL, // Almas y click en Juicio de Almas.
		SOULS_TRIAL,                    // Juicio de Almas y tipos.
		REFINED_EVIL_AS_OBJ_1,          // El objetivo final es refinar el Mal y obtener Refined Evil.
		REFINED_EVIL_AS_OBJ_2,          // El objetivo final es refinar el Mal y obtener Refined Evil.
		ROAD,                           // Carreteras y necesidad de conectar edificios.
		MINE,                           // Mine: localizaci�n y Mineral.
		GAS_PLANT,                      // GasPlant: localizaci�n y Gas.
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
	Manager singleton que centraliza la gesti�n del tutorial. El salto de cada
	etapa a la siguiente ser� realizado a trav�s de la llegada de mensajes:
	
	1) Al recibir ciertos tipos de mensajes (por ejemplo, aquellos de "Edificio construido"),
	   que son condici�n para superar determinadas fases del tutorial, se intentar� avanzar
	   a la siguiente etapa.

	2) As�, para cada uno de esos mensajes, se comprobar� si, efectivamente, el tutorial est�
	   activo y la siguiente fase coincide con aquella a la que se intenta saltar.

	@ingroup logicGroup

	@author Ra�l Segura
	@date Septiembre, 2016
	*/
	class CTutorialManager : public MessageHandler
	{
	public:
		/**
		Devuelve la �nica instancia de la clase.
		@return Puntero a la instancia de la clase.
		*/
		static CTutorialManager *getSingletonPtr() { return _instance; }

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
		Inicializaci�n de los valores de configuraci�n del manager,
		utilizando la informaci�n extra�da de una entidad de mapa
		(Map::CEntity) le�da previamente.

		@param entity Entidad de mapa con los valores de configuraci�n
		le�dos para el manager.
		*/
		void spawn(const Map::CEntity *managerInfo);

		/**
		Inicio del tutorial.
		Llamado desde Logic::CServer::tick.
		*/
		void start();

		/**
		Notificaci�n de bot�n "OK" o "SKIP" pulsado en un evento del
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
		Manejo de mensajes, tiene que manejar todos los tipos de mensajes sin excepci�n.
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
		Comprueba, dada la etapa de entrada (que representa la etapa del
		tutorial finalizada en base a un cierto tipo de mensaje recibido),
		si se debe ejecutar la siguiente fase del tutorial y llama a la
		funci�n encargada de hacerlo en cada caso.

		@return true si se deb�a avanzar a la etapa posterior y el cambio
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
		�nica instancia de la clase.
		*/
		static CTutorialManager *_instance;

		/**
		Tutorial activo, a�n no finalizado ni deshabilitado.
		*/
		bool _active = true;

		/**
		Etapa actual en la que se encuentra el tutorial.
		*/
		TutorialStage _currentStage = TutorialStage::NOT_STARTED;

		/**
		Referencia a la instancia �nica de la clase.
		*/
		GUI::SideBarUI *_sideBarUI = nullptr;

		/**
		TODO Deber�a cogerlas del BuildingManager
		*/
		Ogre::Vector3 hellquartersPosition = Ogre::Vector3(24, 0, 23);
		Ogre::Vector3 evilatorPosition = Ogre::Vector3(18, 0, 20);

	}; // class CTutorialManager

} // namespace Logic

#endif // __TUTORIAL_MANAGER_H