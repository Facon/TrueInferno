/**
@file Life.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el da�o a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	@todo  Si la vida pasa a ser 0 que la entidad muera (poner animaci�n de muerte?)
	y si es el jugador habr� que terminar el juego. Si la vida sigue siendo mayor 
	que 0 tr�s un golpe �poner la animaci�n de herido?.
	
    @ingroup logicGroup

	@author David Llansó Garc�a
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
        RTTI_DECL;
		DEC_FACTORY(CLife);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLife() : IComponent(), _life(100.f) {}
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:

		/**
		Vida de la entidad
		*/
		float _life;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
