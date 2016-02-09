#ifndef TOPBARUI_H_
#define TOPBARUI_H_

namespace CEGUI
{
	class Window;
}

namespace Logic
{
	class ResourcesManager;
	class TimeManager;
}

namespace GUI
{
	class TopBarUI
	{
	protected:
		// Resource Manager used ingame
		Logic::ResourcesManager& _resourceManager;

		// Time of round
		Logic::TimeManager& _timeManager;

		// Window for displaying resources
		CEGUI::Window* _uibarsWindow;
	public:
		TopBarUI(Logic::ResourcesManager& rm, Logic::TimeManager& tm);
		~TopBarUI();
		void init();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
	};
}

#endif // TOPBARUI_H_