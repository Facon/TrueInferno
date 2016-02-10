#include "SideBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>

#include <CEGUI/CEGUI.h>

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"

namespace GUI
{
	SideBarUI::SideBarUI()
	{
	}


	SideBarUI::~SideBarUI()
	{
	}

	CEGUI::Window* SideBarUI::getSideBarWindow(){
		return _uibuttonsWindow;
	}

	void SideBarUI::init()
	{
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIButtonBar.layout");
		_uibuttonsWindow->getChildElement("CreateBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createBuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateRoad")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRoadReleased, this));
	}

	void SideBarUI::activate()
	{
		// Activamos la interfaz de usuario
		_uibuttonsWindow->setVisible(true);
		_uibuttonsWindow->activate();
	}

	void SideBarUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uibuttonsWindow->deactivate();
		_uibuttonsWindow->setVisible(false);
	}

	void SideBarUI::tick(unsigned int msecs)
	{
	}

	bool SideBarUI::createBuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("building");
		return true;

	} //createBuildingReleased

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		printf("road");
		return true;

	} //createBuildingReleased

}