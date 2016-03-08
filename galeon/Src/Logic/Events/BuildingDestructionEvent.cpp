//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.h

Implementación de un evento de tipo INFO consistente en la destrucción de
un edificio de forma aleatoria.

@see Logic::BuildingDestructionEvent

@author Raúl Segura
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
