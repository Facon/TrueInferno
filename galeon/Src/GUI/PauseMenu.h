#ifndef PAUSEMENU_H_
#define PAUSEMENU_H_

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{
	class PauseMenu
	{
	protected:
		// Window for displaying resources
		CEGUI::Window* _menuWindow;
		bool exitReleased(const CEGUI::EventArgs& e);

	public:
		bool _exit = false;

		PauseMenu();
		~PauseMenu();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void close();
		void open();
	};
}

#endif // PAUSEMENU_H_