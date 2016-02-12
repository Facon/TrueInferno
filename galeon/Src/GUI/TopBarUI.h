#ifndef TOPBARUI_H_
#define TOPBARUI_H_

namespace CEGUI
{
	class Window;
}

namespace Logic
{
	class ResourcesManager;
	class HFManager;
	class TimeManager;
}

namespace GUI
{
	class TopBarUI
	{
	protected:
		// Resource Manager used ingame
		Logic::ResourcesManager& _resourceManager;

		// Hades' Favor
		Logic::HFManager& _hfManager;

		// Time of round
		Logic::TimeManager& _timeManager;

		// Window for displaying resources
		CEGUI::Window* _uiBarsWindow;
	public:
		TopBarUI(Logic::ResourcesManager& rm, Logic::HFManager& hfm, Logic::TimeManager& tm);
		~TopBarUI();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		CEGUI::Window* getResourcesWindow();
	};
}

#endif // TOPBARUI_H_