#ifndef SIDEBARUI_H_
#define SIDEBARUI_H_

#include "Logic/Entity/Components/Tile.h"

#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

/**
@author Raúl Segura
*/
namespace SideBar
{
	/**
	Botones de construcción de edificios (y limpieza del terreno) presentes en el menú
	de la barra lateral derecha de la interfaz.
	*/
	enum BuildingButton
	{
		ROAD,
		MINE,
		GAS_PLANT,
		FURNACE,
		EVILWORKS,
		REFINERY,
		POWER_GENERATOR,
		WAREHOUSE,
		CLEAR_TERRAIN
	};

	/**
	Índice entre cada uno de los botones del enum anterior y el nombre (string) que
	hay que pasarle a CEGUI para referenciarlo.
	*/
	typedef boost::unordered_map<SideBar::BuildingButton, std::string> bumap_names;

	const bumap_names buildingButtonsNamesMap = boost::assign::map_list_of
		(BuildingButton::ROAD, "CreateRoad")
		(BuildingButton::MINE, "CreateMine")
		(BuildingButton::GAS_PLANT, "CreateGasPlant")
		(BuildingButton::FURNACE, "CreateFurnace")
		(BuildingButton::EVILWORKS, "CreateEvilworks")
		(BuildingButton::REFINERY, "CreateRefinery")
		(BuildingButton::POWER_GENERATOR, "CreatePowerGenerator")
		(BuildingButton::WAREHOUSE, "CreateWarehouse")
		(BuildingButton::CLEAR_TERRAIN, "ClearTerrain");
}

namespace Logic
{
	class CEntity;
	class Tile;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{

	class SideBarUI
	{
	protected:
		bool _sidebarVisible = true;
		int _redrawUICountResetValue = 1500;
		int _redrawUICountLimit = _redrawUICountResetValue;

		CEGUI::Window* _uibuttonsWindow;
		Logic::CEntity* _placeableEntity=nullptr;
		Logic::CEntity* *_placeableRoad = nullptr;
		int _placeableRoadSize=0;
		Logic::Tile* _originRoadTile=nullptr;
		int _roadInConstruction = false;
		bool _clearTerrain = false;

		bool _firstRoad = true;

		Logic::Tile* _previousOriginRoadTile = nullptr;
		Logic::Tile* _previousToRoadTile = nullptr;
		std::vector<Logic::Tile*>* path = nullptr;
		
		int _tickCountResetValue = 100;
		int _tickCountLimit = _tickCountResetValue;
		bool _firstAnimation = true;

		bool _deactivateCursorAnimation = false;

		// Blinking button
		bool _blinkingButton = false;
		CEGUI::Window* _blinkingButtonFrameWindow = nullptr;

		int _blinkingButtonTickLimit = 500;
		int _blinkingButtonTickCount = _blinkingButtonTickLimit;

		std::string buttonFrame = "TrueInfernoUIBars/ButtonFrame";
		std::string buttonFrameWhite = "TrueInfernoUIBars/ButtonFrameWhite";

		// Botones de construcción de edificios
		bool createRoadReleased(const CEGUI::EventArgs& e);
		bool createMineReleased(const CEGUI::EventArgs& e);
		bool createGasPlantReleased(const CEGUI::EventArgs& e);
		bool createFurnaceReleased(const CEGUI::EventArgs& e);
		bool createEvilworksReleased(const CEGUI::EventArgs& e);
		bool createRefineryReleased(const CEGUI::EventArgs& e);
		bool createPowerGeneratorReleased(const CEGUI::EventArgs& e);
		bool createWarehouseReleased(const CEGUI::EventArgs& e);
		bool clearTerrainReleased(const CEGUI::EventArgs& e);
		
		// Botones de construcción no incluidos finalmente
		bool createResearchLabReleased(const CEGUI::EventArgs& e);
		bool repairBuildingReleased(const CEGUI::EventArgs& e);

		// Botones de almas usados durante el desarrollo
		bool createSoulReleased(const CEGUI::EventArgs& e);
		bool moveSoulReleased(const CEGUI::EventArgs& e);

		// Animación del cursor durante el hover
		bool buttonFrameEnter(const CEGUI::EventArgs& e);
		bool buttonFrameExit(const CEGUI::EventArgs& e);

		// Acciones múltiples
		bool continueRoadCreation();
		bool continueClearTerrain();

		// Reset completo de lo que se estuviera haciendo
		void ClearBuildingConstruction(bool clearPopups);

		// Cambios en botones de construcción bajo demanda
		void buildingButtonShow(SideBar::BuildingButton button);
		void buildingButtonHide(SideBar::BuildingButton button);
		void buildingButtonFadeIn(SideBar::BuildingButton button);
		void buildingButtonFadeOut(SideBar::BuildingButton button);
		void buildingButtonBlinkStart(SideBar::BuildingButton button);
		void buildingButtonBlinkStop(SideBar::BuildingButton button);

		// Función auxiliar para los métodos anteriores
		CEGUI::Window* getButtonWindowFromName(std::string buttonName);

	public:
		bool _onUIScreen = false;
		bool _onButtonFrame = false;
		bool _onGoldFrame = false;
		bool _dropBuilding = false;
		bool _slap = false;
		float _tickCountCursorAnimationResetValue = 300;
		float _tickCountCursorAnimationLimit = _tickCountCursorAnimationResetValue;
		int _cursorAnimationStatus = 1;

		SideBarUI();
		~SideBarUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void playerInteractionWithLeftClick();
		void playerInteractionWithRightClick();
		void setEventWindowVisible(bool visible);
	};
}

#endif // SIDEBARUI_H_