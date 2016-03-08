//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.cpp

Implementaci�n de un evento de tipo INFO consistente en la destrucci�n de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#include "BuildingDestructionEvent.h"
#include "Logic/BuildingManager.h"

namespace Logic {

	void CBuildingDestructionEvent::execute()
	{
		Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();
		// @TODO Mostrar panel de destrucci�n de edificio

	} // execute
	
} // namespace Logic
