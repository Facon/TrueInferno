#include "Movement.h"

#include "KinematicMovement.h"
#include "DynamicMovement.h"

namespace AI {

	/**
	Constante en la que se almacena un valor de tiempo que medir� los 
	milisegundos que queremos que transcurran hasta alcanzar el destino de un
	movimiento.
	*/
	const float IMovement::ATTENUATION = 250.0f;
	/**
	Constante en la que se almacena un valor de distancia que medir� a qu� 
	distancia se empieza a ralentizar el movimiento para llegar a un destino.
	*/
	const float IMovement::SLOW_RADIUS = 30.0f;
	/////////////////////////////////////////
	/**
	M�todo de factor�a que devuelve un movimiento a partir de un enumerado.
	*/
	IMovement* IMovement::getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel)
	{
			switch (type) {
				case MOVEMENT_KINEMATIC_SEEK:
					return  new CKinematicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_KINEMATIC_ARRIVE:
					return  new CKinematicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_KINEMATIC_ALIGN_TO_SPEED:
					return  new CKinematicAlignToSpeed(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_KINEMATIC_FLEE:
					return  new CKinematicFlee(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_DYNAMIC_SEEK:
					return  new CDynamicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_DYNAMIC_ARRIVE:
					return  new CDynamicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				case MOVEMENT_DYNAMIC_FLEE:
					return new CDynamicFlee(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
					break;
				default:
					return 0;
			}
	};
}