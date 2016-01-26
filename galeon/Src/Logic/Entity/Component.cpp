/**
@file Components/Component.cpp

Contiene la implementaci�n de la clase base de los componentes.
 
@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llans�
@date Julio, 2010
*/

#include "Component.h"

//#include "Entity.h"

namespace Logic
{
	RTTI_ROOT_IMPL(IComponent);

	bool IComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		_entity = entity;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs)
	{
		//processMessages();
		// Entity should do the work now

	} // tick

} // namespace Logic
