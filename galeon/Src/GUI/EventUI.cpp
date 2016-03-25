#include "EventUI.h"

#include "Logic/Events/EventManager.h"
#include "Logic/Events/ConditionEvents.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>
#include <Audio/Server.h>

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
		setEventWindowVisible(false);
		_uiEventWindow->activate();
		Audio::CServer::getSingletonPtr()->play();
	}

	void EventUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uiEventWindow->deactivate();
		_uiEventWindow->setVisible(false);
	}

	void EventUI::tick(unsigned int msecs)
	{
		if (firstTick) {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::ConditionEventType::TUTORIAL);
			firstTick = false;
		}		
	}

	bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e)
	{
		// Al pulsar el botón OK simplemente se oculta la ventana.
		_uiEventWindow->setVisible(false);

		// @TODO Hacer esto bien...
		if (firstEventReleased)	{
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::ConditionEventType::TUTORIAL);
			firstEventReleased = false;
		} else if (_uiEventWindow->getChild("EventImage")->getProperty("Image") == "TrueInfernoEvents/EventTutorial4") {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::ConditionEventType::TUTORIAL);
		}

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

	void EventUI::setEventTitle(std::string eventTitle)
	{
		_uiEventWindow->getChild("EventTitle")->setText(eventTitle);
	}

	void EventUI::setEventTextResume(std::string EventTextResume)
	{
		_uiEventWindow->getChild("EventTextResume")->setText(EventTextResume);
	}

	void EventUI::setEventWindowVisible(bool visible)
	{
		_uiEventWindow->setVisible(visible);
	}
	
}