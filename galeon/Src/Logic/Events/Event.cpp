//---------------------------------------------------------------------------
// Event.cpp
//---------------------------------------------------------------------------

/**
@file Event.cpp

Evento de juego. Los eventos se dividirán de dos formas posibles: en función
de su tipo, como eventos de información y eventos de decisión, y en función
de su trigger, como eventos lanzados por tiempo y por condición/acción.

@see Logic::CEventManager

@author Raúl Segura
@date Marzo, 2016
*/

#include "Event.h"

#include "Application/GaleonApplication.h"

#include "BaseSubsystems/ScriptManager.h"

#include <iostream>

namespace Logic {

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

	bool CEvent::launch()
	{
		bool launched = mustBeLaunched();

		if (launched)
			this->execute();

		return launched;

	} // launch

	//--------------------------------------------------------

	bool CEvent::mustBeLaunched()
	{
		// @TODO Usar aquí el TimeManager para los tiempos exactos, ya que el AppTime siempre
		// va por delante tantos segundos como se tarde en cargar la aplicación. Habrá que
		// cambiar la implementación del TimeManager porque ahora mismo solo contiene el
		// tiempo decreciente.
		return (_trigger == TIME && _time <= Application::CGaleonApplication::getSingletonPtr()->getAppTime())
			|| _trigger == CONDITION;

	} // mustBeLaunched
	
} // namespace Logic
