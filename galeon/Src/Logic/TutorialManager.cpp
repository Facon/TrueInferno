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

#include <cassert>

#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/SideBarUI.h"

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

		if (_currentStage == TutorialStage::NOT_STARTED)
			nextStage(_currentStage);

	} // start

	//--------------------------------------------------------

	void CTutorialManager::buttonOK()
	{
		// @TODO Solo habrá que avanzar de esta forma para ciertas etapas/paneles intermedi@s
		// CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);

	} // buttonOK

	//--------------------------------------------------------

	void CTutorialManager::buttonSKIP()
	{
		_currentStage = TutorialStage::SKIPPED;
		endTutorial();

	} // buttonSKIP

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

		return true;

	} // open

	//--------------------------------------------------------

	void CTutorialManager::close()
	{
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
		// @TODO Programar etapas...
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

	} // startStageHellQuartersClick

	//--------------------------------------------------------

	void CTutorialManager::startStageHellQuartersClickSoulsTrial()
	{
	} // startStageHellQuartersClickSoulsTrial

	//--------------------------------------------------------

	void CTutorialManager::startStageSoulsTrial()
	{
	} // startStageSoulsTrial

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinedEvil1()
	{
	} // startStageRefinedEvil1

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinedEvil2()
	{
	} // startStageRefinedEvil2

	//--------------------------------------------------------

	void CTutorialManager::startStageRoad()
	{
	} // startStageRoad

	//--------------------------------------------------------

	void CTutorialManager::startStageMine()
	{
	} // startStageMine

	//--------------------------------------------------------

	void CTutorialManager::startStageGasPlant()
	{
	} // startStageGasPlant

	//--------------------------------------------------------

	void CTutorialManager::startStageFurnace()
	{
	} // startStageFurnace

	//--------------------------------------------------------

	void CTutorialManager::startStageEvilworks()
	{
	} // startStageEvilworks

	//--------------------------------------------------------

	void CTutorialManager::startStageRefinery()
	{
	} // startStageRefinery

	//--------------------------------------------------------

	void CTutorialManager::startStagePowerGenerator()
	{
	} // startStagePowerGenerator

	//--------------------------------------------------------

	void CTutorialManager::startStageWarehouse()
	{
	} // startStageWarehouse

	//--------------------------------------------------------

	void CTutorialManager::endTutorial()
	{
		_active = false;

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
