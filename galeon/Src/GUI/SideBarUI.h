#ifndef SIDEBARUI_H_
#define SIDEBARUI_H_


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

		bool _firstRoad = true;

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
		void ClearBuildingConstruction();
		bool createResearchLabReleased(const CEGUI::EventArgs& e);
		bool createWarehouseReleased(const CEGUI::EventArgs& e);
		bool createPowerGeneratorReleased(const CEGUI::EventArgs& e);

	public:
		SideBarUI();
		~SideBarUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void playerInteractionWithLeftClick();
		void setEventWindowVisible(bool visible);
	};
}

#endif // SIDEBARUI_H_