//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.h

Implementaci�n de un evento de tipo INFO consistente en la destrucci�n de
un edificio de forma aleatoria.

@see Logic::BuildingDestructionEvent

@author Ra�l Segura
@date Marzo, 2016
*/

#include "BuildingDestructionEvent.h"
#include "Logic/BuildingManager.h"
#include <iostream>

namespace Logic {

	void CBuildingDestructionEvent::execute()
	{
		Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();

	} // execute
	
} // namespace Logic
