//---------------------------------------------------------------------------
// TutorialManager.cpp
//---------------------------------------------------------------------------

/**
@file TutorialManager.cpp

Contiene la implementación del gestor del tutorial.

@see Logic::CTutorialManager

@author Raúl Segura
@date Septiembre, 2016
*/

#include "Logic/TutorialManager.h"
#include "Logic/Events/EventManager.h"
#include "Logic/TimeManager.h"

#include "Graphics/Server.h"

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/SideBarUI.h"

#include <cassert>



namespace Logic {

	CTutorialManager* CTutorialManager::_instance = 0;

	//--------------------------------------------------------

	CTutorialManager::CTutorialManager()
	{
		_instance = this;

	} // CTutorialManager

	//--------------------------------------------------------

	CTutorialManager::~CTutorialManager()
	{
		assert(_instance);
		_instance = nullptr;

	} // ~CTutorialManager

	//--------------------------------------------------------

	bool CTutorialManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CTutorialManager no permitida!");

		new CTutorialManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CTutorialManager::Release()
	{
		assert(_instance && "Logic::CTutorialManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CTutorialManager::spawn(const Map::CEntity *managerInfo)
	{
	} // spawn

	//--------------------------------------------------------

	void CTutorialManager::start()
	{
		// Guarda una referencia a la instancia de SideBarUI a través del UIManager
		if (_sideBarUI == nullptr)
			_sideBarUI = GUI::CServer::getSingletonPtr()->getUIManager()->getSideBarUI();

		// Guarda una referencia a la instancia de RankUI a través del UIManager
		if (_rankUI == nullptr)
			_rankUI = GUI::CServer::getSingletonPtr()->getUIManager()->getRankUI();

		// Guarda una referencia a la instancia de TopBarUI a través del UIManager
		if (_topBarUI == nullptr)
			_topBarUI = GUI::CServer::getSingletonPtr()->getUIManager()->getTopBarUI();

		if (_currentStage == TutorialStage::NOT_STARTED)
			nextStage(_currentStage);

	} // start

	//--------------------------------------------------------

	void CTutorialManager::buttonOK()
	{
		// Para determinadas etapas intermedias (con su respectivo panel de evento), se avanza
		// directamente a la siguiente cuando el jugador pulsa el botón OK
		switch (_currentStage)
		{
		case TutorialStage::REFINED_EVIL_AS_OBJ_1:
		case TutorialStage::REFINED_EVIL_AS_OBJ_2:
		case TutorialStage::RANKING:
			nextStage(_currentStage);
			break;
		default:
			// El tutorial se encuentra en cualquier otra etapa que no salta directamente,
			// luego no hacemos nada
			break;
		}

	} // buttonOK

	//--------------------------------------------------------

	void CTutorialManager::buttonSKIP()
	{
		_currentStage = TutorialStage::SKIPPED;

		// Fin del tutorial
		endTutorial();

	} // buttonSKIP

	//--------------------------------------------------------

	void CTutorialManager::buildingSelected(std::string buildingName)
	{
		if (buildingName == "HellQuarters")
			nextStage(TutorialStage::HELLQUARTERS_CLICK);
		else if (buildingName == "Evilator")
			nextStage(TutorialStage::EVILATOR);

	} // buildingSelected

	//--------------------------------------------------------

	void CTutorialManager::soulsTrialSelected()
	{
		nextStage(TutorialStage::HELLQUARTERS_CLICK_SOULS_TRIAL);

	} // soulsTrialSelected

	//--------------------------------------------------------

	void CTutorialManager::soulsTrialCompleted()
	{
		nextStage(TutorialStage::SOULS_TRIAL);

	} // soulsTrialCompleted

	//--------------------------------------------------------

	void CTutorialManager::roadPlaced()
	{
		nextStage(TutorialStage::ROAD);

	} // roadPlaced

	//--------------------------------------------------------

	void CTutorialManager::buildingPlaced(BuildingType buildingType)
	{
		switch (buildingType)
		{
		case Furnace:
			nextStage(TutorialStage::FURNACE);
			break;
		case EvilWorks:
			nextStage(TutorialStage::EVILWORKS);
			break;
		case Refinery:
			nextStage(TutorialStage::REFINERY);
			break;
		case Mine:
			nextStage(TutorialStage::MINE);
			break;
		case GasPlant:
			nextStage(TutorialStage::GAS_PLANT);
			break;
		case PowerGenerator:
			nextStage(TutorialStage::POWER_GENERATOR);
			break;
		case Warehouse:
			nextStage(TutorialStage::WAREHOUSE);
			break;
		default:
			break;
		}

	} // buildingPlaced

	//--------------------------------------------------------

	void CTutorialManager::obstacleRemoved()
	{
		nextStage(TutorialStage::CLEAR_TERRAIN);

	} // obstacleRemoved

	//--------------------------------------------------------

	bool CTutorialManager::open()
	{
		// Eventos del tutorial
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::HELLQUARTERS_CLICK));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::HELLQUARTERS_CLICK_SOULS_TRIAL));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::SOULS_TRIAL));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::REFINED_EVIL_AS_OBJ_1));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::REFINED_EVIL_AS_OBJ_2));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::ROAD));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::MINE));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::GAS_PLANT));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::FURNACE));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::EVILWORKS));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::REFINERY));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::POWER_GENERATOR));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::WAREHOUSE));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::CLEAR_TERRAIN));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::EVILATOR));
		CEventManager::getSingletonPtr()->addConditionEvent(new CTutorialEvent(TutorialStage::RANKING));

		return true;

	} // open

	//--------------------------------------------------------

	void CTutorialManager::close()
	{
		_sideBarUI = nullptr;
		_topBarUI = nullptr;
		_rankUI = nullptr;

	} // close

	//--------------------------------------------------------

	bool CTutorialManager::nextStage(TutorialStage finishedStage)
	{
		if (!_active || _currentStage != finishedStage)
			return false;
		
		// Etapa actual completada con éxito -> Avanzar a la siguiente...
		_currentStage = static_cast<TutorialStage>(_currentStage + 1);

		// ...que siempre tendrá asociado a su inicio un nuevo evento del tutorial
		CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);

		switch (_currentStage)
		{
		case HELLQUARTERS_CLICK:
			startStageHellQuartersClick();
			break;
		case HELLQUARTERS_CLICK_SOULS_TRIAL:
			startStageHellQuartersClickSoulsTrial();
			break;
		case SOULS_TRIAL:
			startStageSoulsTrial();
			break;
		case REFINED_EVIL_AS_OBJ_1:
			startStageRefinedEvil1();
			break;
		case REFINED_EVIL_AS_OBJ_2:
			startStageRefinedEvil2();
			break;
		case ROAD:
			startStageRoad();
			break;
		case MINE:
			startStageMine();
			break;
		case GAS_PLANT:
			startStageGasPlant();
			break;
		case FURNACE:
			startStageFurnace();
			break;
		case EVILWORKS:
			startStageEvilworks();
			break;
		case REFINERY:
			startStageRefinery();
			break;
		case POWER_GENERATOR:
			startStagePowerGenerator();
			break;
		case WAREHOUSE:
			startStageWarehouse();
			break;
		case CLEAR_TERRAIN:
			startStageClearTerrain();
			break;
		case EVILATOR:
			startStageEvilator();
			break;
		case RANKING:
			startStageRanking();
			break;
		case FINISHED:
			endTutorial();
			break;
		default:
			// NOT_STARTED
			// SKIPPED
			// No debería entrar aquí en ningún caso
			return false;
		}

		return true;

	} // nextStage

	//--------------------------------------------------------
	
	void CTutorialManager::startStageHellQuartersClick()
	{
		// Inicio del tutorial
		// Ocultar botones de construcción de edificios, salvo la primera fila
		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::FURNACE);
		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::EVILWORKS);
		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::REFINERY);

		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::POWER_GENERATOR);
		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::WAREHOUSE);
		_sideBarUI->buildingButtonHide(SideBar::BuildingButton::CLEAR_TERRAIN);

		// Iluminar HellQuarters
		Graphics::CServer::getSingletonPtr()->turnOnBuildingLight(hellquartersPosition);

	} // startStageHellQuartersClick

	//--------------------------------------------------------

	void CTutorialManager::startStageHellQuartersClickSoulsTrial()
	{
		// Al llegar aquí se está mostrando el panel del HellQuarters
		// @TODO Blink para el botón del Juicio de Almas

	} // startStageHellQuartersClickSoulsTrial

	//--------------------------------------------------------

	void CTutorialManager::startStageSoulsTrial()
	{
	} // startStageSoulsTrial

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinedEvil1()
	{
		// Dejar de iluminar HellQuarters
		Graphics::CServer::getSingletonPtr()->turnOffBuildingLight();

		// Activar parpadeo para el panel derecho de construcción de edificios
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::BUILDING_BUTTONS);

	} // startStageRefinedEvil1

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinedEvil2()
	{
		// Desactivar parpadeo del panel derecho de construcción de edificios
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::BUILDING_BUTTONS);

		// Activar parpadeo para los paneles superiores de recursos
		_topBarUI->resourcesBlinkStart();

	} // startStageRefinedEvil2

	//--------------------------------------------------------

	void CTutorialManager::startStageRoad()
	{

		// Desactivar parpadeo de los paneles superiores de recursos
		_topBarUI->resourcesBlinkStop();

		// Activar parpadeo para los botones de construcción de edificios (1 a 1)...
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::ROAD);

	} // startStageRoad

	//--------------------------------------------------------

	void CTutorialManager::startStageMine()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::ROAD);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::MINE);

		// Simulamos click derecho del jugador para que salga del modo de construcción
		// de carreteras
		// _sideBarUI->playerInteractionWithRightClick();

		// TODO Estaría bien, pero hay que hacerlo de otra forma porque al llegar a
		// este punto aún no ha hecho el CPlaceable::place() de los tiles de la
		// carretera recién construida

	} // startStageMine

	//--------------------------------------------------------

	void CTutorialManager::startStageGasPlant()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::MINE);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::GAS_PLANT);

	} // startStageGasPlant

	//--------------------------------------------------------

	void CTutorialManager::startStageFurnace()
	{
		// Mostrar la segunda fila de botones de construcción de edificios
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::FURNACE);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::EVILWORKS);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::REFINERY);

		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::GAS_PLANT);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::FURNACE);

	} // startStageFurnace

	//--------------------------------------------------------

	void CTutorialManager::startStageEvilworks()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::FURNACE);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::EVILWORKS);

	} // startStageEvilworks

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinery()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::EVILWORKS);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::REFINERY);

	} // startStageRefinery

	//--------------------------------------------------------

	void CTutorialManager::startStagePowerGenerator()
	{
		// Mostrar la tercera fila de botones de construcción de edificios
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::POWER_GENERATOR);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::WAREHOUSE);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::CLEAR_TERRAIN);

		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::REFINERY);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::POWER_GENERATOR);

	} // startStagePowerGenerator

	//--------------------------------------------------------

	void CTutorialManager::startStageWarehouse()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::POWER_GENERATOR);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::WAREHOUSE);

	} // startStageWarehouse

	//--------------------------------------------------------

	void CTutorialManager::startStageClearTerrain()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::WAREHOUSE);
		_sideBarUI->buildingButtonBlinkStart(SideBar::BuildingButton::CLEAR_TERRAIN);

	} // startStageClearTerrain

	//--------------------------------------------------------

	void CTutorialManager::startStageEvilator()
	{
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::CLEAR_TERRAIN);

		// Simulamos click derecho del jugador para que salga del modo de limpieza
		// del terreno
		_sideBarUI->playerInteractionWithRightClick();

		// Iluminar Evilator
		Graphics::CServer::getSingletonPtr()->turnOnBuildingLight(evilatorPosition);

	} // startStageEvilator

	//--------------------------------------------------------

	void CTutorialManager::startStageRanking()
	{
		// Activar parpadeo para el panel derecho correspondiente al ranking de dioses
		_rankUI->godsRankingBlinkStart();

	} // startStageRanking

	//--------------------------------------------------------

	void CTutorialManager::endTutorial()
	{
		_active = false;

		// Acabe como y cuando acabe, nos aseguramos de que se cierren todos los posibles paneles
		// activos y de que se muestran los botones de construcción de edificios...
		GUI::CServer::getSingletonPtr()->getUIManager()->getBuildingSelectionUI()->DisablePopupVisibility();

		// ...y de que realmente están todos visibles...
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::FURNACE);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::EVILWORKS);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::REFINERY);

		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::POWER_GENERATOR);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::WAREHOUSE);
		_sideBarUI->buildingButtonShow(SideBar::BuildingButton::CLEAR_TERRAIN);

		// ...y de que ninguno parpadea...
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::ROAD);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::MINE);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::GAS_PLANT);

		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::FURNACE);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::EVILWORKS);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::REFINERY);

		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::POWER_GENERATOR);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::WAREHOUSE);
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::CLEAR_TERRAIN);

		// ...y de que el resto de paneles tampoco parpadean...
		_sideBarUI->buildingButtonBlinkStop(SideBar::BuildingButton::BUILDING_BUTTONS);
		_rankUI->godsRankingBlinkStop();
		_topBarUI->resourcesBlinkStop();

		// ...y de que no hay ningún edificio iluminado.
		Graphics::CServer::getSingletonPtr()->turnOffBuildingLight();

		// Activamos el tiempo, con lo que comienza la primera ronda.
		CTimeManager::getSingletonPtr()->setPause(false);

	} // endTutorial

	//--------------------------------------------------------

	bool CTutorialManager::HandleMessage(const Message& msg)
	{
		assert(!"Estás enviando un Message sin tipo.");
		return false;
	}

	bool CTutorialManager::HandleMessage(const TransformMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const PositionMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const RotationMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const DimensionsMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ColorMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const MaterialMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const AnimationMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ControlMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const PhysicMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const TouchMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const DamageMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const WorkerMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const WalkSoulPathMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const HellQuartersMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const MovePlaceableMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const SoulSenderMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const SoulMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const CheckValidPositionPlaceableMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const SoulBurnMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ResourceMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const GetCostPlaceableMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const LogisticsMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ToggleMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const IconMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const PowerMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ConsumptionMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const SoundMessage& msg)
	{
		return false;
	}

	bool CTutorialManager::HandleMessage(const ParticleMessage& msg)
	{
		return false;
	}

} // namespace Logic
