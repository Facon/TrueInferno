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
#include "Logic/BuildingManager.h"
#include "Logic/Server.h"

//includes pal raycast
#include "Graphics/Scene.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneQuery.h>
#include "Graphics/Server.h"
//#include <OgreSceneManager.h>
#include "Graphics/Camera.h"
#include <string>
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"
#include "Physics/Server.h"
#include "Logic\Entity\Message.h"

namespace GUI
{
	SideBarUI::SideBarUI()
	{
	}


	SideBarUI::~SideBarUI()
	{
	}

	void SideBarUI::init()
	{
		_uibuttonsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIButtonBar.layout");

		_uibuttonsWindow->getChildElement("CreateFurnace")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createFurnaceReleased, this));

		_uibuttonsWindow->getChildElement("CreateRoad")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRoadReleased, this));

		_uibuttonsWindow->getChildElement("CreateResource1Building")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createResource1BuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateResource2Building")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createResource2BuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateSoul")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createSoulReleased, this));

		_uibuttonsWindow->getChildElement("MoveSoul")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::moveSoulReleased, this));

		_uibuttonsWindow->getChildElement("CreateEvilworks")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createEvilworksReleased, this));

		_uibuttonsWindow->getChildElement("CreateRefinery")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRefineryReleased, this));

		_uibuttonsWindow->getChildElement("RepairBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::repairBuildingReleased, this));

		_uibuttonsWindow->getChildElement("ClearTerrain")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::clearTerrainReleased, this));
	}

	void SideBarUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uibuttonsWindow);
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
		if (_placeableEntity){
			Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
			//auto mSceneMgr = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();

			float width = (float) mCamera->getViewportWidth(); // viewport width
			float height = (float) mCamera->getViewportHeight(); // viewport height

			//Ogre::RaySceneQuery* mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
			CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
			CEGUI::Vector2f mousePos =
				context.getMouseCursor().getPosition();
			
			Ogre::Ray mouseRay =
				mCamera->getCameraToViewportRay(
				mousePos.d_x / width,
				mousePos.d_y / height);
			/*mRayScnQuery->setRay(mouseRay);
			mRayScnQuery->setSortByDistance(true);

			Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
			Ogre::RaySceneQueryResult::iterator it = result.begin();

			for (; it != result.end(); it++)
			{
				std::string aux = it->movable->getName().c_str();
				std::size_t found = aux.find("Tile");
				if (found != std::string::npos){
					//printf(it->movable->getName().c_str());
					
					//Logic::Tile* tile = Ogre::any_cast<Logic::Tile*>(it->movable->getUserObjectBindings().getUserAny());

					Logic::CBuildingManager::getSingletonPtr()->movePlaceable(Logic::CServer::getSingletonPtr()->getMap(), _placeableEntity, Logic::CTileManager::getSingletonPtr()->getTileByName(it->movable->getName().c_str())->getLogicPosition());
		
					break;
				}
			}*/

			Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(mouseRay, 1000, 1);
			if (entity){
				//printf(entity->getName().c_str());
				Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, entity->getPosition());
			}



			
		}
	}

	bool SideBarUI::createFurnaceReleased(const CEGUI::EventArgs& e)
	{

		//Map::CMapParser::TEntityList mapEntityList =
			//Map::CMapParser::getSingletonPtr()->getEntityList();
		//Logic::CEntityFactory* entityFactory = Logic::CEntityFactory::getSingletonPtr();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Evilator", Vector3(0, 2, 0), true);

		return true;
	} //createBuildingReleased

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 2, 0), true);
		return true;

	} //createBuildingReleased

	bool SideBarUI::createResource1BuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Resource1\n");
		return true;
	}

	bool SideBarUI::createResource2BuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Resource2\n");
		return true;
	}


	bool SideBarUI::createSoulReleased(const CEGUI::EventArgs& e)
	{
		Logic::HellQuartersMessage m(Logic::HellQuartersAction::SEND_SOUL_WORK, 1);
		Logic::CPlaceable* hellQuarters = Logic::CBuildingManager::getSingletonPtr()->findBuilding(Logic::BuildingType::HellQuarters);
		
		m.Dispatch(*hellQuarters->getEntity());
		
		return true;
	}


	bool SideBarUI::moveSoulReleased(const CEGUI::EventArgs& e)
	{
		printf("Move Soul\n");
		return true;
	}


	bool SideBarUI::createEvilworksReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Evilworks\n");
		return true;
	}


	bool SideBarUI::createRefineryReleased(const CEGUI::EventArgs& e)
	{
		printf("Create Refinery\n");
		return true;
	}


	bool SideBarUI::repairBuildingReleased(const CEGUI::EventArgs& e)
	{
		printf("Repair Building\n");
		return true;
	}


	bool SideBarUI::clearTerrainReleased(const CEGUI::EventArgs& e)
	{
		printf("Clear Terrain\n");
		return true;
	}


	void SideBarUI::placeBuildingInConstruction(){
		if (_placeableEntity){
			//Logic::CBuildingManager::getSingletonPtr()->registerBuilding(_placeableEntity->);
			_placeableEntity = nullptr;
		}
	}
}