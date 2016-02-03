/**
@file PhysicController.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include <PxPhysicsAPI.h>
#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

using namespace Logic;
using namespace Physics;
using namespace physx;

RTTI_ROOT_IMPL(CPhysicController);
IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(), _controller(NULL), 
								       _movement(0,0,0), _falling(false)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	_server = NULL;
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);

	return true;
}

//---------------------------------------------------------
/*
bool CPhysicController::accept(const TMessage &message)
{
	return message._type == Message::AVATAR_WALK //|| message._type == Message::SET_TRANSFORM;
} 

//---------------------------------------------------------

void CPhysicController::process(const TMessage &message)
{
	switch(message._type)
	{
	case Message::AVATAR_WALK:
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
		_movement = message._vector3;
		break;
	    //case Message::SET_TRANSFORM:
		//_server->setControllerPosition(_controller, message._transform.getTrans());
		//break;
	}

}
*/
//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al m�todo de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
	// informaci�n proporcionada por el motor de f�sica	
	_entity->setPosition(_server->getControllerPosition(_controller));

	// Si estamos cayendo modificar el vector de desplazamiento para simular el 
	// efecto de la gravedad. Lo hacemos de manera sencilla y pero poco realista.
	if (_falling) {
		_movement += Vector3(0,-1,0);
	}

	// Intentamos mover el controller a la posici�n recibida en el �ltimo mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = _server->moveController(_controller, _movement, msecs);

	// Actualizamos el flag que indica si estamos cayendo
	_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

	// Ponemos el movimiento a cero
	_movement = Vector3::ZERO;
}

//---------------------------------------------------------

PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posici�n de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulaci�n.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisi�n del controller. Por ahora s�lo admitimos c�psulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la c�psula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la c�psula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Crear el controller de tipo c�psula
	return _server->createCapsuleController(position, radius, height, this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad est�tica no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor()->isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinem�tica no hacemos nada
	if (_server->isKinematic(actor))
		return;
	
	// Aplicar una fuerza a la entidad en la direcci�n del movimiento
	actor->addForce(hit.dir * hit.length * 1000.0f);
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}

//---------------------------------------------------------



