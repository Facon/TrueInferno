#ifndef EVENTUI_H_
#define EVENTUI_H_

#include <string>
#include <map>

#include "Logic\Events\Event.h"

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
		bool showingEventList = false;

		int _redrawUICountResetValue = 1500;
		int _redrawUICountLimit = _redrawUICountResetValue;

		CEGUI::Window* _uiEventWindow;
		CEGUI::Window* _uiEventNotification;
		bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e);
		bool EventUI::ShowEventListReleased(const CEGUI::EventArgs& e);
		bool EventUI::showFullEventReleased(const CEGUI::EventArgs& e);
		bool EventUI::ShowEventList();
		bool buttonFrameEnter(const CEGUI::EventArgs& e);
		bool buttonFrameExit(const CEGUI::EventArgs& e);

	public:
		EventUI();
		~EventUI();

		void init();
		void release();
		void activate();
		void deactivate();
		void tick(unsigned int msecs);
		void DisablePopupVisibility();

		/** 
		Registra un evento en el EventUI.
		NOTA: Esta clase asume la responsabilidad de mantener y liberar la memoria del evento.
		 */
		void registerEvent(Logic::CEvent* event);

	private:
		std::map<int, Logic::CEvent*> _events;

		void setEventImage(std::string eventImage);
		void setEventText(std::string eventText);
		void setEventTitle(std::string EventTitle);
		void setEventTextResume(std::string EventTextResume);
		void setEventWindowVisible(bool visible);
		void setEventNotificationVisible(bool visible);

		/**	Muestra el popup completo con la información del evento */
		void showFullEvent(Logic::CEvent* event);
		bool closeWindowReleased(const CEGUI::EventArgs& e);
		/**	Muestra la notificación de un evento */
		void showEventNotification();
		
	};
}

#endif // EVENTUI_H_