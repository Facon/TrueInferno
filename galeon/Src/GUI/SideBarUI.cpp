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
		_uibuttonsWindow->getChildElement("CreateBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createBuildingReleased, this));

		_uibuttonsWindow->getChildElement("CreateRoad")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRoadReleased, this));
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
				Logic::CBuildingManager::getSingletonPtr()->movePlaceable(Logic::CServer::getSingletonPtr()->getMap(), _placeableEntity, entity->getPosition());
			}



			
		}
	}

	bool SideBarUI::createBuildingReleased(const CEGUI::EventArgs& e)
	{

		//Map::CMapParser::TEntityList mapEntityList =
			//Map::CMapParser::getSingletonPtr()->getEntityList();
		//Logic::CEntityFactory* entityFactory = Logic::CEntityFactory::getSingletonPtr();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Evilator", Vector3(0, 2, 0));

		return true;
	} //createBuildingReleased

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 2, 0));
		return true;

	} //createBuildingReleased

	void SideBarUI::placeBuildingInConstruction(){
		if (_placeableEntity){
			//Logic::CBuildingManager::getSingletonPtr()->registerBuilding(_placeableEntity->);
			_placeableEntity = nullptr;
		}
	}
}