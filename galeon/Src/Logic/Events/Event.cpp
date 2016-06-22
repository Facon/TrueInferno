//---------------------------------------------------------------------------
// Event.cpp
//---------------------------------------------------------------------------

/**
@file Event.cpp

Evento de juego. Los eventos se dividir�n de dos formas posibles: en funci�n
de su tipo, como eventos de informaci�n y eventos de decisi�n, y en funci�n
de su trigger, como eventos lanzados por tiempo y por condici�n/acci�n.

@see Logic::CEventManager

@author Ra�l Segura
@date Marzo, 2016
*/

#include "Event.h"

#include "Logic/TimeManager.h"
#include "BaseSubsystems/ScriptManager.h"
#include "GUI/Server.h"
#include "GUI/UIManager.h"
#include "GUI/EventUI.h"
#include "AI/Manager/AIManager.h"

#include <iostream>
#include <Audio/Server.h>

namespace Logic {
	/** Inicializador est�tico del ID de evento */
	int CEvent::_nextEventId = 0;

	/**
	Construye un evento con trigger basado en tiempo.
	@param type Tipo del evento.
	@param time En funci�n del par�metro absoluteTime, instante temporal absoluto en el que se lanzar� el evento
	o relativo al instante actual.
	@absoluteTime Flag a true para lanzar el evento en un instante temporal absoluto o relativo al instante actual.
	@showImmediately Flag a true si el evento debe mostrarse inmediatamente en vez de notificarse primero
	*/
	CEvent::CEvent(EventType type, unsigned long time, bool absoluteTime, bool showImmediately, const std::string& godName) :
		_type(type), _trigger(TIME), _showImmediately(showImmediately) {

		// Si el tiempo es absoluto
		if (absoluteTime)
			// El instante de lanzamiento es el proporcionado
			_time = time;

		// Si no
		else // !absoluteTime
			// El instante de lanzamiento es relativo al instante actual
			_time = Logic::CTimeManager::getSingletonPtr()->getElapsedGlboalTime() + time;

		if (!godName.empty())
			_god = AI::CAIManager::getSingletonPtr()->getGod(godName);
		else
			_god = nullptr;

		_eventId = _nextEventId++;
	}

	/**
	Construye un evento con trigger basado en condici�n.
	*/
	CEvent::CEvent(EventType type, ConditionTriggerType conditionType, bool showImmediately, const std::string& godName) :
		_type(type), _trigger(CONDITION), _conditionType(conditionType), _showImmediately(showImmediately) {

		if (!godName.empty())
			_god = AI::CAIManager::getSingletonPtr()->getGod(godName);
		else
			_god = nullptr;

		_eventId = _nextEventId++;
	}

	void CEvent::luaRegister()
	{
		luabind::module(ScriptManager::CScriptManager::GetPtrSingleton()->getNativeInterpreter())
			[
				luabind::class_<CEvent>("CEvent")
				.enum_("ConditionTriggerType")
				[
					luabind::value("TUTORIAL", CEvent::ConditionTriggerType::TUTORIAL),
					luabind::value("END_GAME", CEvent::ConditionTriggerType::END_GAME)
				]
			];
	}

	//--------------------------------------------------------

	bool CEvent::launch(bool& keepAlive)
	{
		// Si hay que lanzar el evento
		bool launched = mustBeLaunched();
		if (launched){
			// Ejecutamos
			this->execute();

			// Notificamos el evento al EventUI si es necesario
			if (mustBeNotified()) {
				GUI::EventUI *eventUI = GUI::CServer::getSingletonPtr()->getUIManager()->getEventUI();
				eventUI->registerEvent(this);
			}

			Audio::CServer::getSingletonPtr()->playSfxSound("event");

			// TODO Definir por evento/tipo de lanzamiento
			keepAlive = true;
		}

		return launched;

	} // launch

	//--------------------------------------------------------

	bool CEvent::mustBeLaunched()
	{
		// Se usa el CTimeManager para los tiempos exactos, ya que el AppTime siempre
		// va por delante tantos segundos como se tarde en cargar la aplicaci�n.
		return (_trigger == TIME && _time <= Logic::CTimeManager::getSingletonPtr()->getElapsedGlboalTime())
			|| _trigger == CONDITION;

	} // mustBeLaunched

	bool CEvent::mustBeNotified() const {
		// Simplemente se revisa si hay imagen definida. Se podr�a mejorar definiendo un campo adicional a rellenar por cada evento
		return !getGUIImageName().empty();
	}

} // namespace Logic
