//---------------------------------------------------------------------------
// BuildingDestructionEvent.cpp
//---------------------------------------------------------------------------

/**
@file BuildingDestructionEvent.cpp

Implementación de un evento de tipo INFO consistente en la destrucción de
un edificio de forma aleatoria.

@see Logic::CEvent

@author Raúl Segura
@date Marzo, 2016
*/

#include "BuildingDestructionEvent.h"
#include "Logic/BuildingManager.h"

namespace Logic {

	void CBuildingDestructionEvent::execute()
	{
		Logic::CBuildingManager::getSingletonPtr()->DestroyRandomBuilding();
		// @TODO Mostrar panel de destrucción de edificio

	} // execute
	
} // namespace Logic
