#ifndef SIDEBARUI_H_
#define SIDEBARUI_H_


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

		bool createBuildingReleased(const CEGUI::EventArgs& e);

		bool createRoadReleased(const CEGUI::EventArgs& e);

	public:
		SideBarUI();
		~SideBarUI();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
	};
}

#endif // SIDEBARUI_H_