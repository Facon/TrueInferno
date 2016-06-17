#include "EventUI.h"

#include "Logic/Events/EventManager.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/CEGUI.h>
#include <Audio/Server.h>

#include "UIManager.h"
#include "GUI/Server.h"

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
		_uiEventNotification = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventNotification.layout");
		_uiEventNotification->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::ShowEventListReleased, this));
	}

	void EventUI::release()
	{
		// Remove all events to avoid memory leaks
		_uiEventWindow->getChildElement("AcceptEvent")->removeAllEvents();

		// Liberamos los CEvent
		for (auto it = _events.begin(); it != _events.end(); ++it){
			delete (it->second);
			it->second = NULL;
			_events.erase(it);
		}
		_events.clear();
	}

	void EventUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventNotification);
		setEventWindowVisible(false);
		setEventNotificationVisible(false);
		_uiEventWindow->activate();
		_uiEventNotification->activate();
		Audio::CServer::getSingletonPtr()->playSound("audio_hito_3", 0.2f);
	}

	void EventUI::deactivate()
	{
		// Desactivamos la ventana de UiBars.
		_uiEventWindow->deactivate();
		_uiEventWindow->setVisible(false);
		_uiEventNotification->deactivate();
		_uiEventNotification->setVisible(false);
	}

	void EventUI::tick(unsigned int msecs)
	{
		if (firstTick) {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
			firstTick = false;
		}
		if (_events.size()==0)
			setEventNotificationVisible(false);
	}

	bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e)
	{

		// Al pulsar el botón OK simplemente se oculta la ventana.
		_uiEventWindow->setVisible(false);

		// @TODO Hacer esto bien...
		if (firstEventReleased)	{
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
			firstEventReleased = false;
		} else if (_uiEventWindow->getChild("EventImage")->getProperty("Image") == "TrueInfernoEvents/EventTutorial4") {
			Logic::CEventManager::getSingletonPtr()->launchConditionEvent(Logic::CEvent::ConditionTriggerType::TUTORIAL);
		}

		return true;
	}

	void EventUI::registerEvent(Logic::CEvent* event){
		// Mostramos inmediatamente el evento si es necesario
		if (event->getShowImmediately()){
			showFullEvent(event);
		}

		// En caso contrario
		else {
			// Guardamos el evento para mostrarlo más adelante
			_events[event->getEventId()] = event;

			// Se muestra únicamente la notificación
			showEventNotification();
		}
	}
	
	void EventUI::showFullEvent(Logic::CEvent* event){
		GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		setEventImage(event->getGUIImageName());
		setEventText(event->getGUIText());
		setEventTitle(event->getGUITitle());

		setEventTextResume(event->getGUIResumeText());
		setEventWindowVisible(true);
	}

	bool EventUI::showFullEventReleased(const CEGUI::EventArgs& e){
		GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		const CEGUI::MouseEventArgs a = static_cast<const CEGUI::MouseEventArgs&>(e);
		showingEventList = false;
		int i = atoi(a.window->getParent()->getText().c_str());
		Logic::CEvent* event = _events[i];
		

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uiEventWindow);
		_uiEventWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventWindow.layout");
		_uiEventWindow->getChildElement("AcceptEvent")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::AcceptEventReleased, this));
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);

		setEventImage(event->getGUIImageName());
		setEventText(event->getGUIText());
		setEventTitle(event->getGUITitle());
		setEventTextResume(event->getGUIResumeText());
		setEventWindowVisible(true);

		std::map<int, Logic::CEvent*>::iterator it;
		it = _events.find(i);
		_events.erase(it);
		if (_events.size() > 0)
			showEventNotification();

		return true;
	}

	void EventUI::showEventNotification(){
		// TODO Ruymajo's CEGI powa aquí! :D
		_uiEventNotification->getChild("Accept")->setText("* " + std::to_string(_events.size()));
		setEventNotificationVisible(true);
		if (showingEventList){
			ShowEventList();
		}
	}


	bool EventUI::closeWindowReleased(const CEGUI::EventArgs& e)
	{
		showingEventList = false;
		setEventWindowVisible(false);
		return true;
	}

	bool EventUI::ShowEventList()
	{
		GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		showingEventList = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uiEventWindow);
		_uiEventWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventList.layout");

		//_uiEventWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
		//	CEGUI::SubscriberSlot(&EventUI::closeWindowReleased, this));

		float verticalpos = 0.25;
		int i = 1;
		for (auto it = _events.begin(); it != _events.end(); ++it)
		{
			_uiEventWindow->createChild("OgreTray/StaticImage", "frameid" + std::to_string(i));
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("Area", "{{0.17,0},{" + std::to_string(verticalpos - 0.005) + ",0},{0.83,0},{" + std::to_string(verticalpos + 0.065) + ",0}}");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("Image", "TrueInfernoUIBars/ButtonFrame");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("HorzFormatting", "Stretched");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("VertFormatting", "Stretched");

			_uiEventWindow->createChild("OgreTray/StaticText", "id" + std::to_string(i));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Text", std::to_string(it->second->getEventId()));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Alpha", "000");
			_uiEventWindow->getChild("id" + std::to_string(i))->createChild("OgreTray/TrueInfernoButtonText", "Event" + std::to_string(i));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Area", "{{0.2,0},{" + std::to_string(verticalpos) + ",0},{0.8,0},{" + std::to_string(verticalpos + 0.06) + ",0}}");

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Area", "{{0,0},{0,0},{1,0},{1,0}}");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setInheritsAlpha(false);
			_uiEventWindow->getChild("id" + std::to_string(i))->getChildElement("Event" + std::to_string(i))->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&EventUI::showFullEventReleased, this));
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Font", "Jura-10");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Text", it->second->getGUITitle());

			verticalpos += 0.07;
			++i;
			if (i == 9)
				break;
		}

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
		return true;
	}

	bool EventUI::ShowEventListReleased(const CEGUI::EventArgs& e)
	{
		GUI::CServer::getSingletonPtr()->getUIManager()->getSideBarUI()->playerInteractionWithRightClick();
		GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		showingEventList = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->removeChild(_uiEventWindow);
		_uiEventWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventList.layout");

		//_uiEventWindow->getChildElement("CloseWindow")->subscribeEvent(CEGUI::PushButton::EventClicked,
			//CEGUI::SubscriberSlot(&EventUI::closeWindowReleased, this));

		float verticalpos = 0.25;
		int i = 1;
		for (auto it = _events.begin(); it != _events.end(); ++it)
		{
			_uiEventWindow->createChild("OgreTray/StaticImage", "frameid" + std::to_string(i));
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("Area", "{{0.17,0},{" + std::to_string(verticalpos - 0.005) + ",0},{0.83,0},{" + std::to_string(verticalpos + 0.065) + ",0}}");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("Image", "TrueInfernoUIBars/ButtonFrame");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("HorzFormatting", "Stretched");
			_uiEventWindow->getChild("frameid" + std::to_string(i))->setProperty("VertFormatting", "Stretched");

			_uiEventWindow->createChild("OgreTray/StaticText", "id" + std::to_string(i));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Text", std::to_string(it->second->getEventId()));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Alpha", "000");
			_uiEventWindow->getChild("id" + std::to_string(i))->createChild("OgreTray/TrueInfernoButtonText2", "Event" + std::to_string(i));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Area", "{{0.2,0},{" + std::to_string(verticalpos) + ",0},{0.8,0},{" + std::to_string(verticalpos + 0.06) + ",0}}");

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Area", "{{0,0},{0,0},{1,0},{1,0}}");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setInheritsAlpha(false);

			_uiEventWindow->getChild("id" + std::to_string(i))->getChildElement("Event" + std::to_string(i))->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&EventUI::showFullEventReleased, this));

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Font", "Jura-10");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Text", it->second->getGUITitle());

			verticalpos += 0.07;
			++i;
			if (i == 9)
				break;
		}

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
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

	void EventUI::setEventNotificationVisible(bool visible)
	{
		_uiEventNotification->setVisible(visible);
	}

	void EventUI::DisablePopupVisibility(){
		_uiEventWindow->setVisible(false);
		showingEventList = false;
	}
	
}