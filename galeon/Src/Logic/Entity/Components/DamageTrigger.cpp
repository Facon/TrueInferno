/**
@file DamageTrigger.cpp

Contiene la implementaci�n del componente que envia un mensaje DAMAGED cuando su 
entidad es tocada. El mensaje se env�a a la entidad que se ha tocado.
 
@see Logic::CDamageTrigger
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "DamageTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CDamageTrigger);
	IMP_FACTORY(CDamageTrigger);
	
	//---------------------------------------------------------

	bool CDamageTrigger::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("damage"))
			_damage = entityInfo->getFloatAttribute("damage");

		return true;

	} // spawn

	//---------------------------------------------------------
	/*
	bool CDamageTrigger::accept(const MessageType &message)
	{
		return message._type == Message::TOUCHED;

	} // accept
	
	//---------------------------------------------------------

	void CDamageTrigger::process(const MessageType &message)
	{
		switch(message._type)
		{
		case Message::TOUCHED:
			{
				MessageType m;
				m._type = Message::DAMAGED;
				m._float = _damage;
				message._entity->emiMessageType(m);

				std::cout << message._entity->getName()<< std::endl;
			}
			break;
		}

	} // process
	*/
} // namespace Logic

