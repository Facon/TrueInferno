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
		// Window for displaying resources
		CEGUI::Window* _uiBarsWindow;
	public:
		TopBarUI();
		~TopBarUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
	};
}

#endif // TOPBARUI_H_