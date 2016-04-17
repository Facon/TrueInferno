//---------------------------------------------------------------------------
// EventManager.cpp
//---------------------------------------------------------------------------

/**
@file EventManager.cpp

Contiene la implementación del gestor de eventos del juego. Los eventos serán
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CScriptManager

@author Raúl Segura
@date Marzo, 2016
*/

#include "EventManager.h"
#include "BuildingDestructionEvent.h"
#include "EndGameEvent.h"
#include "SoulsSpeedReductionEvent.h"
#include "TutorialEvent.h"

#include "Application/GaleonApplication.h"
#include "BaseSubsystems/ScriptManager.h"
#include "Logic/Entity/Message.h"

#include <cassert>

namespace Logic {

	CEventManager* CEventManager::_instance = 0;

	//--------------------------------------------------------

	CEventManager::CEventManager()
	{
		_instance = this;
		_timeEvents = TEventVector();
		_conditionEvents = TConditionEventMap();

	} // CEventManager

	//--------------------------------------------------------

	CEventManager::~CEventManager()
	{
		assert(_instance);
		_instance = 0;

	} // ~CEventManager

	//--------------------------------------------------------

	bool CEventManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CEventManager no permitida!");

		new CEventManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CEventManager::Release()
	{
		assert(_instance && "Logic::CEventManager no está inicializado!");

		if (_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	void CEventManager::tick(unsigned int msecs)
	{
		CEvent *ev;
		bool launch = true;

		while (!_timeEvents.empty() && launch)
		{
			ev = _timeEvents.front();
			launch = ev->launch();

			if (launch) {
				_timeEvents.erase(_timeEvents.begin());
				delete ev;
				ev = NULL;
			}
		}

	} // tick

	//--------------------------------------------------------

	bool CEventManager::loadEventsScript(const std::string& filename)
	{
		if (!ScriptManager::CScriptManager::GetPtrSingleton()->loadScript(filename.c_str()))
			return false;

		return true;

	} // loadEventsScript

	//--------------------------------------------------------

	void CEventManager::unloadEvents()
	{
		clearTimeEventsQueue();
		clearConditionEventsMap();

	} // unloadEvents

	//--------------------------------------------------------
	
	bool CEventManager::addTimeEvent(CEvent* ev)
	{
		if (ev->getEventTrigger() != CEvent::EventTrigger::TIME ||
				ev->getTime() < Application::CGaleonApplication::getSingletonPtr()->getAppTime())
			return false;

		unsigned long eventTime = ev->getTime();
		auto it = _timeEvents.begin();

		for (it; it != _timeEvents.end(); ++it) {
			if (eventTime < (*it)->getTime())
				break;
		}

		_timeEvents.insert(it, ev);
		return true;

	} // addTimeEvent

	//--------------------------------------------------------
	
	bool CEventManager::addConditionEvent(CEvent* ev)
	{
		if (ev->getEventTrigger() != CEvent::EventTrigger::CONDITION)
			return false;

		_conditionEvents[ev->getConditionTriggerType()].push_back(ev);
		return true;

	} // addConditionEvent

	//--------------------------------------------------------
	
	bool CEventManager::launchConditionEvent(CEvent::ConditionTriggerType conditionTriggerType)
	{
		std::list<CEvent*> eventsList = _conditionEvents[conditionTriggerType];

		if (!eventsList.empty())
		{
			CEvent* conditionEvent = eventsList.front();
			eventsList.pop_front();
			_conditionEvents[conditionTriggerType] = eventsList;

			return conditionEvent->launch();
		}

		return false;

	} // launchConditionEvent

	//--------------------------------------------------------

	bool CEventManager::open() {
		luaRegister();
		loadEventsScript("EventManager.lua");

		return true;

	} // open

	//--------------------------------------------------------

	void CEventManager::close() {
	} // close

	//--------------------------------------------------------

	void CEventManager::luaRegister() {
		// Jerarquía de eventos.
		CEvent::luaRegister();

		CBuildingDestructionEvent::luaRegister();
		CEndGameEvent::luaRegister();
		CSoulsSpeedReductionEvent::luaRegister();
		CTutorialEvent::luaRegister();

		// EventManager.
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CEventManager>("CEventManager")
				.def("addTimeEvent", &CEventManager::addTimeEvent)
				.def("addConditionEvent", &CEventManager::addConditionEvent)
				.def("launchConditionEvent", &CEventManager::launchConditionEvent)
				.scope
				[
					luabind::def("getSingletonPtr", &CEventManager::getSingletonPtr)
				]
			];

		// Jerarquía de mensajes.
		Logic::Message::luaRegister();

	} // luaRegister

	//--------------------------------------------------------

	void CEventManager::clearTimeEventsQueue() {
		while (!_timeEvents.empty())
		{
			CEvent* ev = _timeEvents.front();
			_timeEvents.erase(_timeEvents.begin());

			delete ev;
			ev = NULL;
		}

	} // clearTimeEventsQueue

	//--------------------------------------------------------

	void CEventManager::clearConditionEventsMap() {
		for (auto it = _conditionEvents.begin(); it != _conditionEvents.end(); ++it)
		{
			std::list<CEvent*> eventsList = it->second;

			while (!eventsList.empty())
			{
				CEvent* ev = eventsList.front();
				eventsList.pop_front();

				delete ev;
				ev = NULL;
			}

			eventsList.clear();
		}

		_conditionEvents.clear();

	} // clearConditionEventsMap

	//--------------------------------------------------------

	bool CEventManager::HandleMessage(const Message& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const TransformMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const PositionMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const RotationMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const DimensionsMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const ColorMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const MaterialMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const AnimationMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const ControlMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const PhysicMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const TouchMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const DamageMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const WorkerMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const WalkSoulPathMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const HellQuartersMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const MovePlaceableMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const SoulSenderMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const SoulMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const CheckValidPositionPlaceableMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const NumberMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const ResourceMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const GetCostPlaceableMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const LogisticsMessage& msg)
	{ return false; }

	bool CEventManager::HandleMessage(const ToggleMessage& msg)
	{ return false; }

} // namespace Logic
