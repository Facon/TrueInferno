#ifndef EVENTUI_H_
#define EVENTUI_H_

#include <string>

namespace CEGUI
{
	class Window;
	class EventArgs;
}

namespace GUI
{
	class EventUI
	{
	protected:
		bool firstTick = true;
		bool firstEventReleased = true;

		CEGUI::Window* _uiEventWindow;
		bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e);

	public:
		EventUI();
		~EventUI();
		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void setEventImage(std::string eventImage);
		void setEventText(std::string eventText);
		void setEventTitle(std::string EventTitle);
		void setEventTextResume(std::string EventTextResume);
		void setEventWindowVisible(bool visible);
	};
}

#endif // EVENTUI_H_