#include "BuildingSelectionUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "UIManager.h"
#include "GUI/Server.h"

#include "Logic/Events/EventManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/BuildingSelection.h"
#include "Logic/BuildingManager.h"
#include "Logic/SoulsTrialManager.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Logic/Maps/Managers/WorkManager.h"

namespace GUI
{
	BuildingSelectionUI::BuildingSelectionUI()
	{
	}


	BuildingSelectionUI::~BuildingSelectionUI()
	{
	}

	void BuildingSelectionUI::changeLayout(){
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uibuttonsWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uipopupWindow);
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile(_buildingEntity->getComponent<Logic::CBuildingSelection>()->getSidebarLayoutTemplate());
		_uipopupWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile(_buildingEntity->getComponent<Logic::CBuildingSelection>()->getPopupLayoutTemplate());
		bindButtons();
		loadAssets();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uibuttonsWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uipopupWindow);
	}

	void BuildingSelectionUI::changePopupLayout(std::string layout, std::string name, std::string image){
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uipopupWindow);
		_uipopupWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile(layout);
		bindPopupButtons(layout);
		loadAssetsPopup(name, image);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uipopupWindow);
	}

	void BuildingSelectionUI::bindPopupButtons(std::string layout){

		if (layout == "UIBuildingSelectionPopupHellquarters.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));
			_uipopupWindow->getChildElement("Gods")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::godsReleased, this));
			_uipopupWindow->getChildElement("Missions")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::missionsReleased, this));
			_uipopupWindow->getChildElement("Trial")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::trialReleased, this));
			_uipopupWindow->getChildElement("WhiteMarket")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::whiteMarketReleased, this));
			_uipopupWindow->getChildElement("Redemption")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::redemptionReleased, this));
			_uipopupWindow->getChildElement("BuildingGroups")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::buildingGroupsReleased, this));
		}

		if (layout == "UIBuildingSelectionPopupTrial.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
			_uipopupWindow->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::judgeSoulsReleased, this));

		}

		if (layout == "UIBuildingSelectionPopupBuildingGroups.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
			_uipopupWindow->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::setBuildingGroupsReleased, this));

		}

		if (layout == "UIBuildingSelectionPopupWhiteMarket.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
			_uipopupWindow->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::whiteTradeSoulsReleased, this));

		}

		if (layout == "UIBuildingSelectionPopupRedemption.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
			_uipopupWindow->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::redemptSoulsReleased, this));

		}

		if (layout == "UIBuildingSelectionPopupGods.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
		}

		if (layout == "UIBuildingSelectionPopupMissions.layout"){
			_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::returnToBuildingReleased, this));
		}
	}

	void BuildingSelectionUI::bindButtons(){

		_uibuttonsWindow->getChildElement("UpgradeBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::upgradeBuildingReleased, this));

		_uibuttonsWindow->getChildElement("DisableBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::disableBuildingReleased, this));


		_uibuttonsWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		if (_buildingEntity->getComponent<Logic::CBuildingSelection>()->getSidebarLayoutTemplate() == "UIBuildingSelectionButtonBar.layout"){
			_uibuttonsWindow->getChildElement("DestroyBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::destroyBuildingReleased, this));
		}

		if (_buildingEntity->getComponent<Logic::CBuildingSelection>()->getPopupLayoutTemplate() == "UIBuildingSelectionPopupHellquarters.layout"){
			_uipopupWindow->getChildElement("Gods")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::godsReleased, this));
			_uipopupWindow->getChildElement("Missions")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::missionsReleased, this));
			_uipopupWindow->getChildElement("Trial")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::trialReleased, this));
			_uipopupWindow->getChildElement("WhiteMarket")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::whiteMarketReleased, this));
			_uipopupWindow->getChildElement("Redemption")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::redemptionReleased, this));
			_uipopupWindow->getChildElement("BuildingGroups")->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&BuildingSelectionUI::buildingGroupsReleased, this));
		}
	}

	void BuildingSelectionUI::loadAssets(){
		_uipopupWindow->getChild("BuildingName")->setText(_buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingName());
		_uipopupWindow->getChild("BuildingImage")->setProperty("Image", "TrueInfernoBuildings/" + _buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingImage());

	}

	void BuildingSelectionUI::loadAssetsPopup(std::string name, std::string image)
	{
		_uipopupWindow->getChild("BuildingName")->setText(name);
		
		if (name == "Souls Trial")
		{
			//CEGUI::Editbox* editbox = (CEGUI::Editbox*)_uipopupWindow->getChild("HeavySoulsBurn");
			unsigned int* getAvailableSouls = Logic::CSoulsTrialManager::getSingletonPtr()->getAvailableSouls();
			_uipopupWindow->getChild("HeavySoulTotal")->setText(std::to_string(getAvailableSouls[Logic::SoulsTrialManager::HEAVY]));
			_uipopupWindow->getChild("WastedSoulTotal")->setText(std::to_string(getAvailableSouls[Logic::SoulsTrialManager::WASTED]));
			_uipopupWindow->getChild("LightSoulTotal")->setText(std::to_string(getAvailableSouls[Logic::SoulsTrialManager::LIGHT]));
			_uipopupWindow->getChild("UnknownSoulTotal")->setText(std::to_string(getAvailableSouls[Logic::SoulsTrialManager::UNKNOWN]));
			return;
		}

		if (name == "White Market")
		{
			return;
		}

		if (name == "Building Groups Priorities")
		{
			Logic::BuildingGroup* groupsPriority = Logic::CWorkManager::getSingletonPtr()->getGroupsPriority();
			_uipopupWindow->getChild("PowerSupplyPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::PowerSupply] + 1));
			_uipopupWindow->getChild("MapResourcesPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::MapResources] + 1));
			_uipopupWindow->getChild("ProductionPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::Production] + 1));
			_uipopupWindow->getChild("EvilProcessingPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::EvilProcessing] + 1));
			_uipopupWindow->getChild("LogisticsPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::Logistics] + 1));
			_uipopupWindow->getChild("IDiPriority")->setText(std::to_string(groupsPriority[Logic::BuildingGroup::IDi] + 1));
			return;
		}

		if (name == "Soul Redemption")
		{
			return;
		}

		_uipopupWindow->getChild("BuildingImage")->setProperty("Image", "TrueInfernoBuildings/" + image);
	}

	void BuildingSelectionUI::init()
	{
		
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIBuildingSelectionButtonBar.layout");
		_uipopupWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIBuildingSelectionPopup.layout");

		_uibuttonsWindow->getChildElement("UpgradeBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::upgradeBuildingReleased, this));

		_uibuttonsWindow->getChildElement("DestroyBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::destroyBuildingReleased, this));

		_uibuttonsWindow->getChildElement("DisableBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::disableBuildingReleased, this));

		_uibuttonsWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		
	}

	void BuildingSelectionUI::release()
	{
		// Remove all events to avoid memory leaks
		_uibuttonsWindow->getChildElement("UpgradeBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("DestroyBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("DisableBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CloseWindow")->removeAllEvents();
		_uipopupWindow->getChildElement("CloseWindow")->removeAllEvents();
	}

	void BuildingSelectionUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uibuttonsWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uipopupWindow);
		_uipopupWindow->setVisible(false);
		_uibuttonsWindow->setVisible(false);
		_uibuttonsWindow->activate();
		_uipopupWindow->activate();
	}

	void BuildingSelectionUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uibuttonsWindow->deactivate();
		_uibuttonsWindow->setVisible(false);
		_uipopupWindow->deactivate();
		_uipopupWindow->setVisible(false);
	}


	void BuildingSelectionUI::tick(unsigned int msecs)
	{
	}

	bool BuildingSelectionUI::godsReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupGods.layout", "Gods", "Gods");
		return true;
	}

	bool BuildingSelectionUI::missionsReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupMissions.layout", "Missions", "Missions");
		return true;
	}

	bool BuildingSelectionUI::trialReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupTrial.layout", "Souls Trial", "Trial");
		return true;
	}

	bool BuildingSelectionUI::buildingGroupsReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupBuildingGroups.layout", "Building Groups Priorities", "BuildingGroups");
		return true;
	}

	bool BuildingSelectionUI::whiteMarketReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupWhiteMarket.layout", "White Market", "WhiteMarket");
		return true;
	}

	bool BuildingSelectionUI::redemptionReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout("UIBuildingSelectionPopupRedemption.layout", "Soul Redemption", "Redemption");
		return true;
	}


	bool BuildingSelectionUI::upgradeBuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("upgrade pushed\n");
		return true;
	}

	bool BuildingSelectionUI::disableBuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("disable pushed\n");
		return true;
	}

	bool BuildingSelectionUI::destroyBuildingReleased(const CEGUI::EventArgs& e)
	{
		Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_buildingEntity);
		closeWindow();
		return true;
	}

	bool BuildingSelectionUI::closeWindowReleased(const CEGUI::EventArgs& e)
	{
		closeWindow();
		return true;
	}

	bool BuildingSelectionUI::returnToBuildingReleased(const CEGUI::EventArgs& e)
	{
		changePopupLayout(_buildingEntity->getComponent<Logic::CBuildingSelection>()->getPopupLayoutTemplate(), _buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingName(), _buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingImage());
		return true;
	}

	// TODO Comentar cuando funcione el Juicio de Almas para evitar confusiones
	/*bool BuildingSelectionUI::createSoulReleased(const CEGUI::EventArgs& e)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_WORK, 1, Logic::SoulsTrialManager::SoulsCategory::UNKNOWN);
		Logic::CPlaceable* hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

		return true;
	}

	// TODO Comentar cuando funcione el Juicio de Almas para evitar confusiones
	bool BuildingSelectionUI::burnSoulReleased(const CEGUI::EventArgs& e)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersMessage::HellQuartersAction::SEND_SOUL_BURN, 1, Logic::SoulsTrialManager::SoulsCategory::UNKNOWN);
		Logic::CPlaceable* hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);

		m.Dispatch(*hellQuarters->getEntity());

		return true;
	}*/

	bool BuildingSelectionUI::judgeSoulsReleased(const CEGUI::EventArgs& e)
	{
		unsigned int soulstowork[4];
		unsigned int soulstoburn[4];

		soulstowork[Logic::SoulsTrialManager::HEAVY] = std::atoi(_uipopupWindow->getChild("HeavySoulsWork")->getText().c_str());
		soulstowork[Logic::SoulsTrialManager::WASTED] = std::atoi(_uipopupWindow->getChild("WastedSoulsWork")->getText().c_str());
		soulstowork[Logic::SoulsTrialManager::LIGHT] = std::atoi(_uipopupWindow->getChild("LightSoulsWork")->getText().c_str());
		soulstowork[Logic::SoulsTrialManager::UNKNOWN] = std::atoi(_uipopupWindow->getChild("UnknownSoulsWork")->getText().c_str());

		soulstoburn[Logic::SoulsTrialManager::HEAVY] = std::atoi(_uipopupWindow->getChild("HeavySoulsBurn")->getText().c_str());
		soulstoburn[Logic::SoulsTrialManager::WASTED] = std::atoi(_uipopupWindow->getChild("WastedSoulsBurn")->getText().c_str());
		soulstoburn[Logic::SoulsTrialManager::LIGHT] = std::atoi(_uipopupWindow->getChild("LightSoulsBurn")->getText().c_str());
		soulstoburn[Logic::SoulsTrialManager::UNKNOWN] = std::atoi(_uipopupWindow->getChild("UnknownSoulsBurn")->getText().c_str());

		std::vector<Logic::SoulsTrialManager::SoulsCategory> wrongCategories =
			Logic::CSoulsTrialManager::getSingletonPtr()->createSouls(soulstowork, soulstoburn);

		if (wrongCategories.empty())
				closeWindow();
		else
		{
			_uipopupWindow->getChild("HeavySoulTotal")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");
			_uipopupWindow->getChild("HeavySoulText")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");

			_uipopupWindow->getChild("WastedSoulTotal")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");
			_uipopupWindow->getChild("WastedSoulText")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");

			_uipopupWindow->getChild("LightSoulTotal")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");
			_uipopupWindow->getChild("LightSoulText")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");

			_uipopupWindow->getChild("UnknownSoulTotal")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");
			_uipopupWindow->getChild("UnknownSoulText")->setProperty("TextColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000");

			for (auto it = wrongCategories.begin(); it != wrongCategories.end(); ++it)
			{
				if ((*it) == Logic::SoulsTrialManager::SoulsCategory::HEAVY)
				{
					_uipopupWindow->getChild("HeavySoulTotal")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
					_uipopupWindow->getChild("HeavySoulText")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
				}
				if ((*it) == Logic::SoulsTrialManager::SoulsCategory::WASTED)
				{
					_uipopupWindow->getChild("WastedSoulTotal")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
					_uipopupWindow->getChild("WastedSoulText")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
				}
				if ((*it) == Logic::SoulsTrialManager::SoulsCategory::LIGHT)
				{
					_uipopupWindow->getChild("LightSoulTotal")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
					_uipopupWindow->getChild("LightSoulText")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
				}
				if ((*it) == Logic::SoulsTrialManager::SoulsCategory::UNKNOWN)
				{
					_uipopupWindow->getChild("UnknownSoulTotal")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
					_uipopupWindow->getChild("UnknownSoulText")->setProperty("TextColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
				}
			}

		}

		return true;
	}

	bool BuildingSelectionUI::whiteTradeSoulsReleased(const CEGUI::EventArgs& e)
	{
		printf("white market traded\n");
		return true;
	}

	bool BuildingSelectionUI::redemptSoulsReleased(const CEGUI::EventArgs& e)
	{
		printf("souls redemptioned\n");
		return true;
	}

	bool BuildingSelectionUI::setBuildingGroupsReleased(const CEGUI::EventArgs& e)
	{
		printf("building group properties set\n");
		return true;
	}

	bool checkPlaceableUpgradeCost(Logic::CEntity* placeableEntity){
		if (!placeableEntity){
			std::cout << "Can't check consume costs on empty placeable" << std::endl;
			return false;
		}

		Logic::GetCostPlaceableMessage m(Logic::MessageType::PLACEABLE_CONSUME_COST, placeableEntity);

		return m.Dispatch(*placeableEntity);
	}

	void BuildingSelectionUI::setEventWindowVisible(bool visible, Logic::CEntity* entity)
	{	
		_buildingEntity = entity;
		changeLayout();
		_uibuttonsWindow->setVisible(visible);
		_uipopupWindow->setVisible(visible);
	}

	void BuildingSelectionUI::closeWindow(){
		_buildingEntity = nullptr;
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->setEventWindowVisible(true);
		_uibuttonsWindow->setVisible(false);
		_uipopupWindow->setVisible(false);
	}
}