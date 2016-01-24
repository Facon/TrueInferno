/**
@file RayPerception.cpp

Contiene la implementaci�n de un componente de percepci�n basado en lanzamiento de rayos.
 
@see Logic::CRayPerception
@see Logic::IComponent

@author Antonio A. S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "RayPerception.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

using namespace Logic;

RTTI_ROOT_IMPL(CRayPerception);
IMP_FACTORY(CRayPerception);
	
//---------------------------------------------------------

CRayPerception::CRayPerception() : IComponent(), _distance(0.0f) 
{

}

//---------------------------------------------------------

CRayPerception::~CRayPerception()
{

}

//---------------------------------------------------------

bool CRayPerception::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Leer distancia m�xima a la que se perciben las entidades (por defecto 10)
	_distance = 10.0f;
	if(entityInfo->hasAttribute("perception_distance"))
		_distance = entityInfo->getFloatAttribute("perception_distance");

	return true;
} 
	
//---------------------------------------------------------

void CRayPerception::tick(unsigned int msecs)
{
	// Invocar al m�todo de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Vamos a lanzar el rayo a mitad de altura de la c�psula del jugador.
	// Para que no golpee contra la propia c�psula debe empezar justo delante
	// de ella.
	// HACK: altura y anchura de la c�psula deber�an estar parametrizadas
	Vector3 dir = Math::getDirection(_entity->getTransform());
	Vector3 origin = _entity->getPosition() + Vector3(0, 3, 0) + 3.1f * dir;

	// Crear el rayo rayo y comprobar si colisiona con alguna entidad
	Ray ray(origin, dir);
	CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, _distance);
	if (entity) {
		//std::cout << entity->getName() << " percibida" << std::endl;
	}

	// Nota: este componente es bastante in�til tal y como est� programado. Habr�a que
	// generalizarlo para que funcionase con otras entidades.
	
	// Nota: tambi�n podr�amos lanzar el rayo a altura del suelo. En ese caso no necesitamos
	// conocer el tama�o de la c�psula puesto que nunca la golpear�a (empieza fuera de ella).
	// El problema es que el suelo de Gale�n no es completamente liso (por ejemplo en las puertas).
}

//---------------------------------------------------------



