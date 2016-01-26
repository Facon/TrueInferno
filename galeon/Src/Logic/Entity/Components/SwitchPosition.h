/**
@file SwitchPosition.h

Contiene la declaraci�n del componente que controla el movimiento de
una entidad que tiene dos estado (posiciones) para ir de una a otra.

@see Logic::CSwitchPosition
@see Logic::IComponent

@author David Llans�
@date Octubre, 2010
*/
#ifndef __Logic_SwitchPosition_H
#define __Logic_SwitchPosition_H

#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
	/**
	Este componente controla el movimiento de una entidad que tiene dos estados 
	(posiciones) y la mueve de uno a otro.
	<p>
	Las posiciones entre las que se transita pueden configurarse desde el mapa.
	De la misma manera se puede estableces el atributo "loop" que mantendr� constante
	las transiciones. El atributo "speed" determina la velocidad del movimiento.
	<p>
	Recibe mensajes de tipo "SWITCH" para transitar de un estado a otro en caso de
	que no se encuentre en loop. El atributo "_int" del mensaje indica el estado
	al que se debe mover la entidad.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Octubre, 2010
	*/
	class CSwitchPosition : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(CSwitchPosition);
	public:

		/**
		Constructor por defecto.
		*/
		CSwitchPosition();
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Mueve el componente hacia la posici�n de destino. Una vez la alcance se para
		(loop = false) o empieza a moverse hacia la otra posici�n (loop = true).
		*/
		virtual void tick(unsigned int msecs);

	protected:

		/**
		Posici�n inicial (_position[0]) y final (_position[1]).
		*/
		Vector3 _position[2];
	
		/**
		Posici�n hacia la que nos estamos moviendo.
		*/
		Vector3 _targetPosition;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;
		
		/**
		Atributo que indica si estamos constantemente moviendo de un punto a otro.
		*/
		bool _loop;

		/**
		�ltimo movimiento realizado.
		*/
		Vector3 _lastMovement;

	}; // class CSwitchPosition

	REG_FACTORY(CSwitchPosition);

} // namespace Logic

#endif // __Logic_SwitchPosition_H
