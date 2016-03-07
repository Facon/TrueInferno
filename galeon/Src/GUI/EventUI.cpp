#include "EventUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>

namespace GUI
{
	EventUI::EventUI()
	{
	}


	EventUI::~EventUI()
	{
	}

	void EventUI::init()
	{
		_uiEventWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventWindow.layout");
		_uiEventWindow->getChildElement("AcceptEvent")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::AcceptEventReleased, this));
	}

	void EventUI::release()
	{
		// Remove all events to avoid memory leaks
		_uiEventWindow->getChildElement("AcceptEvent")->removeAllEvents();
	}

	void EventUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
		setEventImage("EventTutorial1");
		_uiEventWindow->setVisible(true);
		_uiEventWindow->activate();
	}

	void EventUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uiEventWindow->deactivate();
		_uiEventWindow->setVisible(false);
	}

	void EventUI::tick(unsigned int msecs)
	{
		
	}

	bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e)
	{
		printf("evento aceptado\n");
		_uiEventWindow->setVisible(false);
		return true;
	}

	void EventUI::setEventImage(std::string eventImage)
	{
		_uiEventWindow->getChild("EventImage")->setProperty("Image","TrueInfernoEvents/" + eventImage);
	}
	void EventUI::setEventText(std::string eventText)
	{
		_uiEventWindow->getChild("EventText")->setText(eventText);
	}

	
}