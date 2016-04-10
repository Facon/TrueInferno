#ifndef BUILDINGSELECTIONUI_H_
#define BUILDINGSELECTIONUI_H_

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
		bool closeWindowReleased(const CEGUI::EventArgs& e);
		void BuildingSelectionUI::bindButtons();
		void BuildingSelectionUI::loadAssets();

	public:
		BuildingSelectionUI();
		~BuildingSelectionUI();
		void changeLayout();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void setEventWindowVisible(bool visible, Logic::CEntity* entity);
		void BuildingSelectionUI::closeWindow();
	};
}

#endif // BUILDINGSELECTIONUI_H_