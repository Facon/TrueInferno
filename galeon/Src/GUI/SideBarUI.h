#ifndef SIDEBARUI_H_
#define SIDEBARUI_H_


namespace Logic
{
	class CEntity;
}
//lo de antes ver si kitar
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
		int _Constructing = 0;

		bool createBuildingReleased(const CEGUI::EventArgs& e);

		bool createRoadReleased(const CEGUI::EventArgs& e);

	public:
		SideBarUI();
		~SideBarUI();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void placeBuildingInConstruction();
	};
}

#endif // SIDEBARUI_H_