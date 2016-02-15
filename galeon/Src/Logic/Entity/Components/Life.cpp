/**
@file Life.cpp

Contiene la implementaci�n del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

namespace Logic 
{
	RTTI_ROOT_IMPL(CLife);
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
			_life = entityInfo->getFloatAttribute("life");

		return true;

	} // spawn
	
	//---------------------------------------------------------
	/*
	bool CLife::accept(const Message &message)
	{
		return message._type == Message::DAMAGED;

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(const Message &message)
	{
		switch(message._type)
		{
		case Message::DAMAGED:
			{
				// Disminuir la vida de la entidad
				_life -= message._float;
				printf("Herido\n");

				// Si han matado al jugador salir de la partida
				if ((_life <= 0) && (_entity->isPlayer())) {
					Application::CBaseApplication::getSingletonPtr()->setState("menu");
				}
				// @todo Poner la animaci�n de herido.
				// @todo Si la vida es menor que 0 poner animaci�n de morir.

			}
			break;
		}

	} // process
	*/
} // namespace Logic

