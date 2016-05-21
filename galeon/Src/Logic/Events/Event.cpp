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

#include <iostream>

namespace Logic {

	int CEvent::_nextEventId = 0;

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

			// Notificamos el evento al EventUI
			GUI::EventUI *eventUI = GUI::CServer::getSingletonPtr()->getUIManager()->getEventUI();
			eventUI->registerEvent(this);

			// TODO Definir por evento/tipo de lanzamiento!
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
	/*
	TEventID EventID::_nextId = EventID::FIRST_ID;

	//---------------------------------------------------------

	TEntityID EntityID::NextID()
	{
		TEntityID ret = _nextId;
		assert(ret != EntityID::UNASSIGNED && "Se han asignado todos los identificadores posibles.");
		_nextId++;
		return ret;

	} // NextEntityId
	*/
} // namespace Logic
