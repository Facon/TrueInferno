/**
@file CollisionManager.h

Contiene la declaraci�n del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __PHYSICS_COLLISION_MANAGER_H_
#define __PHYSICS_COLLISION_MANAGER_H_

#include <PxSimulationEventCallback.h> 
#include <characterkinematic/PxController.h>

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades f�sicas. Cuando se produce una
	colisi�n notifica a los componentes l�gicos correspondientes. 

	 @ingroup physicGroup

	 @author Antonio S�nchez Ruiz-Granados
	 @date Noviembre, 2012
	*/
	class CCollisionManager : public physx::PxSimulationEventCallback,
							  public physx::PxUserControllerHitReport
						  
	{
	public:
		/**
		Constructor por defecto.
		*/
		CCollisionManager();

		/**
		Destructor.
		*/
		virtual ~CCollisionManager();

		/**
		M�todo invocado cuando se rompe un constraint (ver documentaci�n PhysX).
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
		/**
		M�todo invocado cuando se despiertan actores f�sicos (ver documentaci�n PhysX).
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);
  
		/**
		M�todo invocado cuando se duermen actores f�sicos (ver documentaci�n PhysX).
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
		/**
		M�todo invocado cuando se produce un contacto entre dos shapes f�sicos 
		(ver documentaci�n PhysX).
		*/
		void onContact(const physx::PxContactPairHeader &pairHeader, 
			           const physx::PxContactPair *pairs, physx::PxU32 nbPairs); 
  
		/**
		M�todo invocado cuando una entidad f�sica entra o sale de un trigger 
		(ver documentaci�n PhysX).
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		/**
		M�todo invocado cuando un character controller golpea una shape 
		(ver documentaci�n PhysX).
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);
  
		/**
		M�todo invocado cuando un character controller golpea otro character controller 
		(ver documentaci�n PhysX).
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit); 
   
		/**
		M�todo invocado cuando un character controller golpea un obst�culo definido por 
		el usuario (ver documentaci�n PhysX).
		*/ 
		void onObstacleHit(const physx::PxControllerObstacleHit &hit);
   
	}; // CCollisionManager

}; // namespace Physics

#endif // __PHYSICS_COLLISION_MANAGER_H_