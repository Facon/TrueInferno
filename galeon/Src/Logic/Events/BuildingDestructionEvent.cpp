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
#include <iostream>

namespace Logic {

	void CBuildingDestructionEvent::execute()
	{
		// @TODO Destruir un edificio aleatorio.
		std::cout << "CBuildingDestructionEvent: " << _time / 1000 << std::endl;

	} // execute
	
} // namespace Logic
