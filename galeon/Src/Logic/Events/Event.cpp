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

#include "Application/GaleonApplication.h"

#include <iostream>

namespace Logic {

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
		// @TODO Usar aqu� el TimeManager para los tiempos exactos, ya que el AppTime siempre
		// va por delante tantos segundos como se tarde en cargar la aplicaci�n. Habr� que
		// cambiar la implementaci�n del TimeManager porque ahora mismo solo contiene el
		// tiempo decreciente.
		return (_trigger == TIME && _time <= Application::CGaleonApplication::getSingletonPtr()->getAppTime())
			|| _trigger == CONDITION;

	} // mustBeLaunched
	
} // namespace Logic
