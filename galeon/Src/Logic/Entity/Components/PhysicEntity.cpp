/**
@file PhysicEntity.cpp

Contiene la implementaci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include <PxPhysicsAPI.h>

#include "BaseSubsystems/Math.h"

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

using namespace Logic;
using namespace Physics;
using namespace physx;

RTTI_ROOT_IMPL(CPhysicEntity);
IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IPhysics(), _actor(NULL), _movement(0,0,0)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicEntity::~CPhysicEntity() 
{
	if (_actor) {
		_server->destroyActor(_actor);
		_actor = NULL;
	}

	_server = NULL;
} 

//---------------------------------------------------------

bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el objeto f�sico asociado al componente
	_actor = createActor(entityInfo);

	return true;
}

//---------------------------------------------------------

void CPhysicEntity::tick(unsigned int msecs)
{
	// Invocar al m�todo de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Si es una entidad est�tica no hacemos nada
	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	if (!dinActor) 
		return;

	// Si es una entidad dinámica: Actualizar el transform de la entidad
	// l�gica usando la informaci�n proporcionada por el motor de f�sica	
	_entity->setTransform(_server->getActorTransform(dinActor));

	// Si el objeto f�sico es cinem�tico, intentamos moverlo de acuerdo 
	// a los mensajes KINEMATIC_MOVE recibidos 
	if (_server->isKinematic(dinActor)) {
		_server->moveKinematicActor(dinActor, _movement);
		_movement = Vector3::ZERO;
	}
}

//---------------------------------------------------------

bool CPhysicEntity::HandleMessage(const TransformMessage& m)
{
	_server->setActorTransform(_actor, m._transform);
	return true;

} // SET_TRANSFORM

//---------------------------------------------------------

bool CPhysicEntity::HandleMessage(const KinematicMoveMessage& m)
{
	_movement += m._shift;
	return true;

} // KINEMATIC_MOVE

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return NULL;
}

//---------------------------------------------------------

PxRigidStatic* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
{
	// La posici�n de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	// Leer el grupo de colisi�n (por defecto grupo 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Crear el plano
	return _server->createPlane(point, normal, group, this);
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
{
	// Leer la posici�n de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: est�ticos, din�mico o cinem�tico
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");
	assert(physicShape == "box");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	// Leer el grupo de colisi�n (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	if (physicType == "static") {
		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja est�tica
			return _server->createStaticBox(position, physicDimensions, trigger, group, this);
		}

	} else {
		// Leer la masa (por defecto 0)
		float mass = 0;
		if (entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");
		
		// Leer si se trata de un actor cinem�tico
		bool kinematic = (physicType == "kinematic");

		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja din�mica
			return _server->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, this);
		}
	}

	return NULL;
}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	// Leer el grupo de colisi�n (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Crear el actor a partir del fichero RepX
	return _server->createFromFile(file, group, this);
}

//---------------------------------------------------------

void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
{
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 
	MessageType type = (enter) ? MessageType::TOUCHED : MessageType::UNTOUCHED;
    CEntity& entity = *otherComponent->getEntity();

	TouchMessage msg(type, entity);

	msg.Dispatch(*_entity);
}
