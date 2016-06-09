#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "GUI/TopBarUI.h"
#include "GUI/SideBarUI.h"
#include "GUI/EventUI.h"
#include "GUI/BuildingSelectionUI.h"
#include "GUI/RankUI.h"

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
		BuildingSelectionUI _buildingSelectionUI;
		RankUI _rankUI;
		// Window for displaying resources
		CEGUI::Window* _uiWindow;
		bool UIManager::HandleRootMouseDown(const CEGUI::EventArgs& e);

	public:
		UIManager();
		~UIManager();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		TopBarUI *getTopBarUI(){ return &_topBarUI; };
		RankUI *getRankUI(){ return &_rankUI; };
		EventUI *getEventUI(){ return &_eventUI; };
		SideBarUI *getSideBarUI(){ return &_sideBarUI; };
		BuildingSelectionUI *getBuildingSelectionUI(){ return &_buildingSelectionUI; };
		void disablePopupWindows();
	};
}

#endif // UIMANAGER_H_