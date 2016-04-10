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
#include "Logic/Events/ConditionEvents.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/Entity/Components/BuildingSelection.h"
#include "Logic/BuildingManager.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"

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

	void BuildingSelectionUI::bindButtons(){

		_uibuttonsWindow->getChildElement("UpgradeBuilding/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::upgradeBuildingReleased, this));


		_uibuttonsWindow->getChildElement("CloseWindow/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		if (_buildingEntity->getComponent<Logic::CBuildingSelection>()->getSidebarLayoutTemplate() == "UIBuildingSelectionButtonBar.layout"){
			_uibuttonsWindow->getChildElement("DestroyBuilding/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::destroyBuildingReleased, this));
		}
	}

	void BuildingSelectionUI::loadAssets(){
		_uipopupWindow->getChild("BuildingName")->setText(_buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingName());
		_uipopupWindow->getChild("BuildingImage")->setProperty("Image", "TrueInfernoEvents/" + _buildingEntity->getComponent<Logic::CBuildingSelection>()->getBuildingImage());

	}

	void BuildingSelectionUI::init()
	{
		
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIBuildingSelectionButtonBar.layout");
		_uipopupWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIBuildingSelectionPopup.layout");

		_uibuttonsWindow->getChildElement("UpgradeBuilding/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::upgradeBuildingReleased, this));

		_uibuttonsWindow->getChildElement("DestroyBuilding/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::destroyBuildingReleased, this));

		_uibuttonsWindow->getChildElement("CloseWindow/Image/Button")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		_uipopupWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&BuildingSelectionUI::closeWindowReleased, this));

		
	}

	void BuildingSelectionUI::release()
	{
		// Remove all events to avoid memory leaks
		_uibuttonsWindow->getChildElement("UpgradeBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("DestroyBuilding")->removeAllEvents();
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

	bool BuildingSelectionUI::upgradeBuildingReleased(const CEGUI::EventArgs& e)
	{
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