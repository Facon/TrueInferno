//---------------------------------------------------------------------------
// EventManager.cpp
//---------------------------------------------------------------------------

/**
@file EventManager.cpp

Contiene la implementación del gestor de eventos del juego. Los eventos serán
creados desde un script de LUA, procesados (en caso de que sea necesario)
y encolados hasta que llegue el momento de su lanzamiento.

@see Logic::CEventManager

@author Raúl Segura
@date Marzo, 2016
*/

#include "EventManager.h"
#include "BuildingDestructionEvent.h"

#include "BaseSubsystems/ScriptManager.h"

#include <cassert>

namespace Logic {

	CEventManager* CEventManager::_instance = 0;

	//--------------------------------------------------------

	CEventManager::CEventManager()
	{
		_instance = this;

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
		CBuildingDestructionEvent *ev;
		bool launch = true;

		while (!_timeEvents.empty() && launch)
		{
			// @TODO Implementar para usar con cualquier tipo de evento.
			ev = dynamic_cast<CBuildingDestructionEvent*>(_timeEvents.front());

			if (ev) {
				launch = ev->launch();

				if (launch) {
					_timeEvents.pop();
					delete ev;
					ev = NULL;
				}
			}
		}

	} // tick

	//--------------------------------------------------------

	bool CEventManager::loadEvents(const std::string& filename)
	{
		// Ejecutar script.
		if (!ScriptManager::CScriptManager::GetPtrSingleton()->loadScript(filename.c_str()))
			return false;

		// @TODO Borrar cuando se carguen desde LUA!
		addTimeEvent(new CBuildingDestructionEvent(20 * 1000));
		addTimeEvent(new CBuildingDestructionEvent(45 * 1000));

		return true;

	} // loadEvents

	//--------------------------------------------------------

	void CEventManager::unloadEvents()
	{
		clearTimeEventsQueue();

	} // unloadEvents

	//--------------------------------------------------------
	
	bool CEventManager::addTimeEvent(CEvent* ev)
	{
		if (ev->getEventTrigger() != CEvent::EventTrigger::TIME)
			return false;

		_timeEvents.push(ev);
		return true;

	} // addTimeEvent

	//--------------------------------------------------------

	bool CEventManager::open() {
		return true;
	} // open

	//--------------------------------------------------------

	void CEventManager::close() {
	} // close

	//--------------------------------------------------------

	void CEventManager::clearTimeEventsQueue() {
		while (!_timeEvents.empty())
		{
			CEvent* ev = _timeEvents.front();
			_timeEvents.pop();

			delete ev;
			ev = NULL;
		}

	} // clearEventsQueue

} // namespace Logic
