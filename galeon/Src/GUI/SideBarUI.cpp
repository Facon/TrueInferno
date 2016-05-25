#include "SideBarUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

#include "GUI/Server.h"
#include "UIManager.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "Logic/Events/EventManager.h"
#include "Logic/Events/Event.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Placeable.h"
#include "Logic/BuildingManager.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/Tile.h"
#include "Logic/Maps/Managers/TileManager.h"

#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include "Graphics/Camera.h"

#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneQuery.h>

#include "Physics/Server.h"
#include "Logic/Entity/Message.h"
#include "AI/Server.h"

#include "Logic\ResourcesManager.h"

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

		_uibuttonsWindow->getChildElement("CreateGasPlant")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createGasPlantReleased, this));

		_uibuttonsWindow->getChildElement("CreateMine")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createMineReleased, this));

		_uibuttonsWindow->getChildElement("CreateEvilworks")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createEvilworksReleased, this));

		_uibuttonsWindow->getChildElement("CreateRefinery")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createRefineryReleased, this));

		_uibuttonsWindow->getChildElement("RepairBuilding")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::repairBuildingReleased, this));

		_uibuttonsWindow->getChildElement("ClearTerrain")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::clearTerrainReleased, this));

		_uibuttonsWindow->getChildElement("CreateResearchLab")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createResearchLabReleased, this));

		_uibuttonsWindow->getChildElement("CreateWarehouse")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createWarehouseReleased, this));

		_uibuttonsWindow->getChildElement("CreatePowerGenerator")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&SideBarUI::createPowerGeneratorReleased, this));
	}

	void SideBarUI::release()
	{
		// Remove all events to avoid memory leaks
		_uibuttonsWindow->getChildElement("CreateFurnace")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateRoad")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateMine")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateGasPlant")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateEvilworks")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateRefinery")->removeAllEvents();
		_uibuttonsWindow->getChildElement("RepairBuilding")->removeAllEvents();
		_uibuttonsWindow->getChildElement("ClearTerrain")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateResearchLab")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreateWarehouse")->removeAllEvents();
		_uibuttonsWindow->getChildElement("CreatePowerGenerator")->removeAllEvents();

		_placeableEntity = nullptr;
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

	Logic::CEntity* getEntityFromRaycastToGroup(int collisiongroup){

		Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();

		float width = (float)mCamera->getViewportWidth();
		float height = (float)mCamera->getViewportHeight();

		CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
		CEGUI::Vector2f mousePos =
			context.getMouseCursor().getPosition();


		Ogre::Ray mouseRay =
			mCamera->getCameraToViewportRay(
			mousePos.d_x / width,
			mousePos.d_y / height);

		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(mouseRay, 1000, collisiongroup);

		return entity;
	}

	void SideBarUI::tick(unsigned int msecs)
	{
		//resize en CEGUI tiene algun fallo con los botones
		//Graphics::CCamera* mCamera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();

		//float width = (float)mCamera->getViewportWidth();
		//float height = (float)mCamera->getViewportHeight();
		//printf("width %f height %f\n", width, height);
		//CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(width,height));

		if (_placeableEntity){

			Logic::CEntity* entity = getEntityFromRaycastToGroup(1);

			if (entity)
			{
				// Obtenemos la Tile sacando el componente directamente
				Logic::Tile* to = entity->getComponent<Logic::Tile>();

				if (_roadInConstruction == 2) // TODO Convendría usar un enum para mejorar legiblidad
				{
					std::vector<Logic::Tile*>* path= AI::CServer::getSingletonPtr()->getSoulPathAStarRoute(_originRoadTile, to, true);

					if (path)
					{
						Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, to->getLogicPosition(), false);
						if (_placeableRoadSize > 0){
							for (int i = 0; i < _placeableRoadSize; ++i){
								if (_placeableRoad[i])
									Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
							}
							_placeableRoadSize = 0;
							free(_placeableRoad);
							_placeableEntity = nullptr;
						}

						int j = 0;

						//borrarñapa (ÁVV: en principio las rutas ya nunca van a tener el primer nodo repetido por lo que esto ya no haría falta)
						/*if (path->size()){
							path->erase(path->cbegin());
						}*/

						//finborrarñapa
						_placeableRoad = new Logic::CEntity*[path->size()];
						_placeableRoadSize = path->size();
						for (auto it = path->cbegin(); it != path->cend(); ++it)
						{
							Logic::Tile* tile = (*it);
							_placeableRoad[j++] = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", tile->getLogicPosition(), true, false);
						}
						_placeableEntity = _placeableRoad[0];

						// Liberamos la ruta
						delete path;
						path = nullptr;
					}
				}
				else{
					Logic::CBuildingManager::getSingletonPtr()->floatPlaceableTo(_placeableEntity, to->getLogicPosition(), true);
				}

			}
		}
	}

	bool SideBarUI::createFurnaceReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Furnace", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createRoadReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 0, 0), true, true);
		if (_placeableEntity){
			_placeableRoad = new Logic::CEntity*[1];
			_placeableRoadSize = 1;
			_placeableRoad[0] = _placeableEntity;
			_roadInConstruction = 1;
		}
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::continueRoadCreation()
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "SoulPath", Vector3(0, 0, 0), true, true);
		if (_placeableEntity){
			_placeableRoad = new Logic::CEntity*[1];
			_placeableRoadSize = 1;
			_placeableRoad[0] = _placeableEntity;
			_roadInConstruction = 1;
		}
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createMineReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Mine", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createGasPlantReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "GasPlant", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createEvilworksReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "EvilWorks", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}


	bool SideBarUI::createRefineryReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Refinery", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::repairBuildingReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding(); // TODO TEST borrar edificio
		printf("Repair Building\n");
		return true;
	}

	bool SideBarUI::clearTerrainReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		// @TODO Cambiar puntero a pala
		_clearTerrain = true;
		return true;
	}

	bool SideBarUI::continueClearTerrain()
	{
		ClearBuildingConstruction();
		// @TODO Cambiar puntero a pala
		_clearTerrain = true;
		return true;
	}

	bool SideBarUI::createResearchLabReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "ResearchLabs", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createWarehouseReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "Warehouse", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	bool SideBarUI::createPowerGeneratorReleased(const CEGUI::EventArgs& e)
	{
		ClearBuildingConstruction();
		_placeableEntity = Logic::CBuildingManager::getSingletonPtr()->createPlaceable(Logic::CServer::getSingletonPtr()->getMap(), "PowerGenerator", Vector3(0, 0, 0), true, true);
		return (_placeableEntity != nullptr);
	}

	void SideBarUI::ClearBuildingConstruction(){
		_clearTerrain = false;
		_roadInConstruction = 0;
		if (_placeableRoadSize > 0){
			for (int i = 0; i < _placeableRoadSize; ++i){
				if (_placeableRoad[i])
					Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
			}
			_placeableRoadSize = 0;
			free(_placeableRoad);
			_placeableEntity = nullptr;
		}
		else if (_placeableEntity){
			Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableEntity);
			_placeableEntity = nullptr;
		}
	}

	bool checkPlaceableConsumeCost(Logic::CEntity* placeableEntity){
		if (!placeableEntity){
			std::cout << "Can't check consume costs on empty placeable" << std::endl;
			return false;
		}

		Logic::GetCostPlaceableMessage m(Logic::MessageType::PLACEABLE_CONSUME_COST, placeableEntity);

		return m.Dispatch(*placeableEntity);
	}

	void SideBarUI::playerInteractionWithRightClick(){
		ClearBuildingConstruction();
	}

	void SideBarUI::playerInteractionWithLeftClick()
	{
		if (_placeableEntity)
		{
			Logic::CEntity* entity = getEntityFromRaycastToGroup(1);
			if (entity){
				switch (_roadInConstruction)
				{
					case 0:
					{
						_originRoadTile = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
						if (Logic::CBuildingManager::getSingletonPtr()->checkValidPlaceablePosition(_placeableEntity, _originRoadTile->getLogicPosition()))
						{
							if (checkPlaceableConsumeCost(_placeableEntity))
								Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableEntity);
							else
								Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableEntity);

							_placeableEntity = nullptr;
							break;
						}
					}
					case 1:
					{
						_originRoadTile = Logic::CTileManager::getSingletonPtr()->getNearestTile(entity->getPosition());
						if (Logic::CBuildingManager::getSingletonPtr()->checkValidPlaceablePosition(_placeableEntity, _originRoadTile->getLogicPosition()))
							_roadInConstruction = 2;
						else
							_originRoadTile = nullptr;
						break;
					}
					case 2:
					{
						for (int i = 0; i < _placeableRoadSize; ++i)
						{
							if (checkPlaceableConsumeCost(_placeableRoad[i]))
								Logic::CBuildingManager::getSingletonPtr()->placePlaceable(_placeableRoad[i]);
							else
								Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(_placeableRoad[i]);
						}
						free(_placeableRoad);
						_placeableRoad = nullptr;
						_placeableEntity = nullptr;
						_roadInConstruction = 0;
						_placeableRoadSize = 0;
						_originRoadTile = nullptr;

						// @TODO Hacer esto bien...
						if (_firstRoad) {
							Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
							_firstRoad = false;
						}
						continueRoadCreation();
						break;
					}
				}
			}
		}
		else{
			if (_clearTerrain){
				Logic::CEntity* entity = getEntityFromRaycastToGroup(3);
				if (entity)
				{
					Logic::CBuildingManager::getSingletonPtr()->destroyPlaceable(entity);
					continueClearTerrain();
				}			
			}
			else
			{
				Logic::CEntity* entity = getEntityFromRaycastToGroup(2);
				if (entity)
				{
					GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
					uiManager->getBuildingSelectionUI()->setEventWindowVisible(true, entity);
					_uibuttonsWindow->setVisible(false);
				}
			}		
		}
	}

	void SideBarUI::setEventWindowVisible(bool visible)
	{
		_uibuttonsWindow->setVisible(visible);
	}

}