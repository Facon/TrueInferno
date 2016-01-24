/**
@file RayPerception.h

Contiene la declaraci�n de un componente de percepci�n basado en lanzamiento de rayos.

@see Logic::CRayPerception
@see Logic::IComponent

@author Antonio A. S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_RayPerception_H
#define __Logic_RayPerception_H

#include "BaseSubsystems/RTTI.h"
#include "Logic/Entity/Component.h"

// Los componentes se definen dentro del namespace Logica
namespace Logic 
{
	/**
	Este componente lanza un rayo en la direcci�n en la que mira la entidad para 
	comprobar si alguna otra entidad est� cerca.

	@todo Ahora mismo este componente s�lo escribe por consola los objetos que
	percibe. En el futuro habr�a que mandar un mensaje a la entidad para que los
	componentes	encargados del comportamiento reaccionen.
	
    @ingroup logicGroup

	@author Antonio A. S�nchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CRayPerception : public IComponent
	{
		RTTI_DECL;
		DEC_FACTORY(CRayPerception);
	public:

		/**
		Constructor por defecto.
		*/
		CRayPerception();

		/**
		Destructor.
		*/
		virtual ~CRayPerception();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		En cada tick lanza un rayo en la direcci�n que mira la entidad para comprobar
		si hay alguna otra entidad en su rango de percepci�n.
		*/
		virtual void tick(unsigned int msecs);

	protected:

		/**
		Distancia m�xima a la que se perciben las entidades.
		*/
		float _distance;

	}; // class CRayPerception

	REG_FACTORY(CRayPerception);

} // namespace Logic

#endif // __Logic_RayPerception_H