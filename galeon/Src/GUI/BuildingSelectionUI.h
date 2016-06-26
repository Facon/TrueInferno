#ifndef BUILDINGSELECTIONUI_H_
#define BUILDINGSELECTIONUI_H_

#include <string>
#include <vector>

namespace Logic
{
	class CEntity;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{
	class BuildingSelectionUI
	{
	protected:
		CEGUI::Window* _uibuttonsWindow;
		CEGUI::Window* _uipopupWindow;
		Logic::CEntity* _buildingEntity = nullptr;

		bool upgradeBuildingReleased(const CEGUI::EventArgs& e);
		bool destroyBuildingReleased(const CEGUI::EventArgs& e);
		bool disableBuildingReleased(const CEGUI::EventArgs& e);
		bool closeWindowReleased(const CEGUI::EventArgs& e);
		bool godsReleased(const CEGUI::EventArgs& e);
		bool missionsReleased(const CEGUI::EventArgs& e);
		bool trialReleased(const CEGUI::EventArgs& e);
		bool buildingGroupsReleased(const CEGUI::EventArgs& e);
		bool whiteMarketReleased(const CEGUI::EventArgs& e);
		bool redemptionReleased(const CEGUI::EventArgs& e);
		bool returnToBuildingReleased(const CEGUI::EventArgs& e);
		//bool createSoulReleased(const CEGUI::EventArgs& e);
		//bool burnSoulReleased(const CEGUI::EventArgs& e);
		bool judgeSoulsReleased(const CEGUI::EventArgs& e);
		bool setBuildingGroupsReleased(const CEGUI::EventArgs& e);
		bool redemptSoulsReleased(const CEGUI::EventArgs& e);
		bool whiteTradeSoulsReleased(const CEGUI::EventArgs& e);
		void bindPopupButtons(std::string layout);
		void bindButtons();
		void loadAssets();
		void loadAssetsPopup(std::string name, std::string image);
		bool buttonFrameEnter(const CEGUI::EventArgs& e);
		bool buttonFrameExit(const CEGUI::EventArgs& e);

	public:
		BuildingSelectionUI();
		~BuildingSelectionUI();
		void changeLayout();
		void changeButtonLayout();
		void changePopupLayout(std::string layout, std::string name, std::string image);
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void setEventWindowVisible(bool visible, Logic::CEntity* entity);
		void closeWindow();
		void DisablePopupVisibility();
	};
}

#endif // BUILDINGSELECTIONUI_H_