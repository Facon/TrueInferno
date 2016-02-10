#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "GUI/ResourcesUI.h"
#include "GUI/SideBarUI.h"

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

		ResourcesUI _resourcesUI;
		SideBarUI _sidebarUI;
		// Window for displaying resources
		CEGUI::Window* _uiWindow;


	public:
		UIManager();
		~UIManager();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
	};
}

#endif // UIMANAGER_H_