/**
@file PhysicEntity.h

Contiene la declaraci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Physics.h"
#include "BaseSubsystems/RTTI.h"

// Predeclaraci�n de tipos
namespace physx {
	class PxRigidActor;
	class PxRigidStatic;
};

namespace Physics {
	class CServer;
};

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representaci�n f�sica de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un �nico actor de PhysX.
	Esta clase no sirve para representar character controllers ni entidades compuestas como
	ragdolls.
	<p>
	En PhysX existen dos tipos de entidades f�sicas: est�ticas y din�micas. Las entidades 
	est�ticas no se pueden mover una vez creadas y s�lo sirven como vol�menes de colisi�n.
	Las entidades din�micas tienen masa y se mueven aplic�ndoles fuerzas (por ejemplo la gravedad). 
	Tambi�n existe un tipo especial de entidades din�micas llamadas cinem�ticas, que est�n pensadas 
	para representar objetos cuyo movimiento viene dictado desde la l�gica del juego. Las entidades 
	cinem�ticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	Solo para entidades dinámicas: En cada tick, este componente actualiza la posici�n, rotaci�n y
	escalado de las entidades l�gicas usando la informaci�n que proporciona el motor de f�sica. 
	<p>
	Cuando este componente se utiliza para representar una entidad cinem�tica, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad f�sica de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author Antonio S�nchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CPhysicEntity : public IPhysics
	{
		RTTI_DECL;
		DEC_FACTORY(CPhysicEntity);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicEntity();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Si la entidad f�sica es de tipo est�tico no hace nada. </li>
		<li> Si la entidad f�sica es de tipo din�mico actualiza posición, rotación y escalado de 
		     la entidad l�gica usando la informaci�n proporcionada por el motor de f�sica. </li>
		<li> Si la entidad f�sica es de tipo cinem�tico, adem�s solicita al motor de f�sica
		     que mueva la entidad de acuerdo al �ltimo mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void tick(unsigned int msecs);

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		*/
		virtual void onTrigger(IPhysics *otherComponent, bool enter);

		/**
		Mensajes.
		*/
		bool HandleMessage(const TransformMessage& m);
		bool HandleMessage(const KinematicMoveMessage& m);

	private:

		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		physx::PxRigidActor* createActor(const Map::CEntity *entityInfo);

		/**
		Crea un plano est�tico a partir de la informaci�n de mapa. 
		*/
		physx::PxRigidStatic* createPlane(const Map::CEntity *entityInfo);
				
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de la informaci�n de mapa. 
		*/
		physx::PxRigidActor* createRigid(const Map::CEntity *entityInfo);
		
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		physx::PxRigidActor* createFromFile(const Map::CEntity *entityInfo);

		// Servidor de f�sica
		Physics::CServer *_server;

		// Actor que representa la entidad f�sica en PhysX
		physx::PxRigidActor *_actor;

		// Vector de deplazamiento recibido en el �ltimo mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades f�sicas cinem�ticas.
		Vector3 _movement;

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
