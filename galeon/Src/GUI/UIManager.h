#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "GUI/TopBarUI.h"
#include "GUI/SideBarUI.h"
#include "GUI/EventUI.h"

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{
	class UIManager
	{
	protected:

		TopBarUI _topBarUI;
		SideBarUI _sideBarUI;
		EventUI _eventUI;
		// Window for displaying resources
		CEGUI::Window* _uiWindow;


	public:
		UIManager(Logic::ResourcesManager& rm, Logic::HFManager& hfm, Logic::TimeManager& tm);
		~UIManager();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		EventUI *getEventUI(){ return &_eventUI; };
		SideBarUI *getSideBarUI(){ return &_sideBarUI; };
	};
}

#endif // UIMANAGER_H_