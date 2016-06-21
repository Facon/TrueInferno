#ifndef SIDEBARUI_H_
#define SIDEBARUI_H_

#include "Logic/Entity/Components/Tile.h"

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
		CEGUI::Window* _uibuttonsWindow;
		Logic::CEntity* _placeableEntity=nullptr;
		Logic::CEntity* *_placeableRoad = nullptr;
		int _placeableRoadSize;
		Logic::Tile* _originRoadTile=nullptr;
		int _roadInConstruction = false;
		bool _clearTerrain = false;

		bool _firstRoad = true;

		Logic::Tile* _previousOriginRoadTile = nullptr;
		Logic::Tile* _previousToRoadTile = nullptr;
		std::vector<Logic::Tile*>* path = nullptr;
		
		int _tickCountResetValue = 100;
		int _tickCountLimit = _tickCountResetValue;

		



		bool createFurnaceReleased(const CEGUI::EventArgs& e);
		bool createRoadReleased(const CEGUI::EventArgs& e);
		bool createMineReleased(const CEGUI::EventArgs& e);
		bool createGasPlantReleased(const CEGUI::EventArgs& e);
		bool createSoulReleased(const CEGUI::EventArgs& e);
		bool moveSoulReleased(const CEGUI::EventArgs& e);
		bool createEvilworksReleased(const CEGUI::EventArgs& e);
		bool createRefineryReleased(const CEGUI::EventArgs& e);
		bool repairBuildingReleased(const CEGUI::EventArgs& e);
		bool clearTerrainReleased(const CEGUI::EventArgs& e);
		void ClearBuildingConstruction(bool clearPopups);
		bool createResearchLabReleased(const CEGUI::EventArgs& e);
		bool createWarehouseReleased(const CEGUI::EventArgs& e);
		bool createPowerGeneratorReleased(const CEGUI::EventArgs& e);
		bool buttonFrameEnter(const CEGUI::EventArgs& e);
		bool buttonFrameExit(const CEGUI::EventArgs& e);

		bool continueRoadCreation();
		bool continueClearTerrain();

	public:
		bool _onUIScreen = false;
		bool _onButtonFrame = false;
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