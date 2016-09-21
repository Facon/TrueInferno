#include "EventUI.h"

#include "Logic/Events/EventManager.h"
#include "Logic/TutorialManager.h"

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
		// Eventos
		_uiEventWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventWindow.layout");
		_uiEventWindow->getChildElement("AcceptEvent")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::AcceptEventReleased, this));

		// Eventos del tutorial
		_uiEventTutorialWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventTutorialWindow.layout");
		_uiEventTutorialWindow->getChildElement("AcceptEvent")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::AcceptEventTutorialReleased, this));
		_uiEventTutorialWindow->getChildElement("SkipEvent")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::SkipEventTutorialReleased, this));

		// Notificación de eventos
		_uiEventNotification = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UIEventNotification.layout");
		_uiEventNotification->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::SubscriberSlot(&EventUI::ShowEventListReleased, this));

		_uiEventNotification->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,
			CEGUI::SubscriberSlot(&EventUI::buttonFrameEnter, this));
		_uiEventNotification->getChildElement("Accept")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,
			CEGUI::SubscriberSlot(&EventUI::buttonFrameExit, this));
	}

	void EventUI::release()
	{
		for (auto it = _events.begin(); it != _events.end(); ++it){
			delete (it->second);
			it->second = nullptr;
		}

		_events.clear();

		_uiEventNotification->getChildElement("Accept")->removeAllEvents();
		_uiEventTutorialWindow->getChildElement("AcceptEvent")->removeAllEvents();
		_uiEventWindow->getChildElement("AcceptEvent")->removeAllEvents();
	}

	void EventUI::activate()
	{
		// Activamos la interfaz de usuario
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventTutorialWindow);
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventNotification);

		setEventWindowVisible(false);
		setEventTutorialWindowVisible(false);
		setEventNotificationVisible(false);

		_uiEventWindow->activate();
		_uiEventTutorialWindow->activate();
		_uiEventNotification->activate();
	}

	void EventUI::deactivate()
	{
		// Desactivamos la ventana de UiBars
		_uiEventWindow->deactivate();
		_uiEventWindow->setVisible(false);

		_uiEventTutorialWindow->deactivate();
		_uiEventTutorialWindow->setVisible(false);

		_uiEventNotification->deactivate();
		_uiEventNotification->setVisible(false);
	}

	void EventUI::tick(unsigned int msecs)
	{
		if (_events.size()==0)
			setEventNotificationVisible(false);

		_redrawUICountLimit -= msecs;
		if (_redrawUICountLimit <= 0)
		{
			_redrawUICountLimit = _redrawUICountResetValue;

			//for (unsigned int i = 0; i < _uiEventWindow->getChildCount(); ++i)
			//	_uiEventWindow->getChildAtIdx(i)->moveToFront();

			//for (unsigned int i = 0; i < _uiEventNotification->getChildCount(); ++i)
			//	_uiEventNotification->getChildAtIdx(i)->moveToFront();
		}
	}

	bool EventUI::AcceptEventReleased(const CEGUI::EventArgs& e)
	{
		// Al pulsar el botón OK simplemente se oculta la ventana.
		_uiEventWindow->setVisible(false);

		return true;
	}

	bool EventUI::AcceptEventTutorialReleased(const CEGUI::EventArgs& e)
	{
		// Al pulsar el botón OK se oculta la ventana y se notifica al TutorialManager
		_uiEventTutorialWindow->setVisible(false);

		Logic::CTutorialManager::getSingletonPtr()->buttonOK();
		return true;
	}

	bool EventUI::SkipEventTutorialReleased(const CEGUI::EventArgs& e)
	{
		// Al pulsar el botón SKIP se oculta la ventana y se notifica al TutorialManager
		_uiEventTutorialWindow->setVisible(false);

		Logic::CTutorialManager::getSingletonPtr()->buttonSKIP();
		return true;
	}

	void EventUI::registerEvent(Logic::CEvent* event){
		// Mostramos inmediatamente el evento si es necesario
		if (event->getShowImmediately())
			showFullEvent(event);

		// En caso contrario
		else {
			// Guardamos el evento para mostrarlo más adelante
			_events[event->getEventId()] = event;

			// Se muestra únicamente la notificación
			showEventNotification();
		}
	}
	
	void EventUI::showFullEvent(Logic::CEvent* event)
	{
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->disablePopupWindows();
		uiManager->getSideBarUI()->_onUIScreen = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");

		if (event->getConditionTriggerType() == Logic::CEvent::ConditionTriggerType::TUTORIAL)
		{
			setEventTutorialTitle(event->getGUITitle());
			setEventTutorialImage(event->getGUIImageName());
			setEventTutorialText(event->getGUIText());
			setEventTutorialWindowVisible(true);
		}
		else
		{
			setEventImage(event->getGUIImageName());
			setEventText(event->getGUIText());
			setEventTitle(event->getGUITitle());
			setEventTextResume(event->getGUIResumeText());
			setEventWindowVisible(true);
		}
	}

	bool EventUI::showFullEventReleased(const CEGUI::EventArgs& e)
	{
		GUI::CServer::getSingletonPtr()->getUIManager()->disablePopupWindows();
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->_onUIScreen = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");

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

	bool EventUI::buttonFrameEnter(const CEGUI::EventArgs& e)
	{
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->_onButtonFrame = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");
		return true;
	}

	bool EventUI::buttonFrameExit(const CEGUI::EventArgs& e)
	{
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->_onButtonFrame = false;
		if (!uiManager->getSideBarUI()->_onUIScreen){
			uiManager->getSideBarUI()->_cursorAnimationStatus = 1;
			uiManager->getSideBarUI()->_tickCountCursorAnimationLimit = uiManager->getSideBarUI()->_tickCountCursorAnimationResetValue;
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoIdleCursors/CursorIdle1");
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoIdleCursors/CursorIdle1");
		}
		return true;
	}

	void EventUI::showEventNotification()
	{
		// TODO Ruymajo's CEGUI powa aquí! :D
		_uiEventNotification->getChild("Accept")->setText("* " + std::to_string(_events.size()));
		setEventNotificationVisible(true);

		if (showingEventList)
			ShowEventList();
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

		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->_onUIScreen = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");
		
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
			_uiEventWindow->getChild("id" + std::to_string(i))->createChild("OgreTray/TrueInfernoButtonText2", "Event" + std::to_string(i));
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Area", "{{0.2,0},{" + std::to_string(verticalpos) + ",0},{0.8,0},{" + std::to_string(verticalpos + 0.06) + ",0}}");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("AlwaysOnTop", "True");

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Area", "{{0,0},{0,0},{1,0},{1,0}}");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setInheritsAlpha(false);
			_uiEventWindow->getChild("id" + std::to_string(i))->getChildElement("Event" + std::to_string(i))->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&EventUI::showFullEventReleased, this));
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Font", "DeadSecretary-12");
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
		GUI::UIManager *uiManager = GUI::CServer::getSingletonPtr()->getUIManager();
		uiManager->getSideBarUI()->_onUIScreen = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("TrueInfernoOtherCursors/CursorPoint");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("TrueInfernoOtherCursors/CursorPoint");
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
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("AlwaysOnTop", "True");
			_uiEventWindow->getChild("id" + std::to_string(i))->setProperty("Area", "{{0.2,0},{" + std::to_string(verticalpos) + ",0},{0.8,0},{" + std::to_string(verticalpos + 0.06) + ",0}}");

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Area", "{{0,0},{0,0},{1,0},{1,0}}");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setInheritsAlpha(false);

			_uiEventWindow->getChild("id" + std::to_string(i))->getChildElement("Event" + std::to_string(i))->subscribeEvent(CEGUI::PushButton::EventClicked,
				CEGUI::SubscriberSlot(&EventUI::showFullEventReleased, this));

			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Font", "DeadSecretary-12");
			_uiEventWindow->getChild("id" + std::to_string(i))->getChild("Event" + std::to_string(i))->setProperty("Text", it->second->getGUITitle());

			verticalpos += 0.07;
			++i;
			if (i == 9)
				break;
		}

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(_uiEventWindow);
		return true;
	}

	//---------------------------------------------

	void EventUI::setEventTitle(std::string eventTitle)
	{
		_uiEventWindow->getChild("EventTitle")->setText(eventTitle);
	}

	void EventUI::setEventImage(std::string eventImage)
	{
		_uiEventWindow->getChild("EventImage")->setProperty("Image","TrueInfernoNewEvents/" + eventImage);
	}
	
	void EventUI::setEventText(std::string eventText)
	{
		_uiEventWindow->getChild("EventText")->setText(eventText);
	}

	void EventUI::setEventTextResume(std::string EventTextResume)
	{
		_uiEventWindow->getChild("EventTextResume")->setText(EventTextResume);
	}

	//---------------------------------------------

	void EventUI::setEventTutorialTitle(std::string eventTitle)
	{
		_uiEventTutorialWindow->getChild("EventTitle")->setText(eventTitle);
	}

	void EventUI::setEventTutorialImage(std::string eventImage)
	{
		_uiEventTutorialWindow->getChild("EventImage")->setProperty("Image", eventImage);
	}

	void EventUI::setEventTutorialText(std::string eventText)
	{
		_uiEventTutorialWindow->getChild("EventText")->setText(eventText);
	}

	//---------------------------------------------

	void EventUI::setEventWindowVisible(bool visible)
	{
		_uiEventWindow->setVisible(visible);
	}

	void EventUI::setEventTutorialWindowVisible(bool visible)
	{
		_uiEventTutorialWindow->setVisible(visible);
	}

	void EventUI::setEventNotificationVisible(bool visible)
	{
		_uiEventNotification->setVisible(visible);
	}

	//---------------------------------------------

	void EventUI::DisablePopupVisibility()
	{
		_uiEventWindow->setVisible(false);
		_uiEventTutorialWindow->setVisible(false);
		showingEventList = false;
	}
	
}